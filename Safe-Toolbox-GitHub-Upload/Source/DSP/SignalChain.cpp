#include "SignalChain.h"

#include <cmath>

#include "SampleSanitizer.h"
#include "../Parameters/ParameterUtilities.h"

namespace
{
    inline void updatePeak (std::atomic<float>& peak, float value) noexcept
    {
        const float current = peak.load (std::memory_order_relaxed);
        if (value > current)
            peak.store (value, std::memory_order_relaxed);
    }
}

void SignalChain::prepare (double newSampleRate, int maximumBlockSize, int numChannels) noexcept
{
    sampleRate = juce::jmax (newSampleRate, 1.0);
    channels = juce::jlimit (1, 2, numChannels);

    dryBuffer.setSize (channels, juce::jmax (1, maximumBlockSize), false, false, true);
    dryBuffer.clear();

    dcBlocker.prepare (sampleRate, channels);
    driveProcessor.prepare (sampleRate, channels);
    filterProcessor.prepare (sampleRate, maximumBlockSize, channels);
    delayProcessor.prepare (sampleRate, maximumBlockSize, channels);
    reverbProcessor.prepare (sampleRate, maximumBlockSize, channels);
    wobbleProcessor.prepare (sampleRate, maximumBlockSize, channels);
    widthProcessor.prepare (sampleRate, maximumBlockSize, channels);
    safetyLimiter.prepare (sampleRate, channels);
}

void SignalChain::reset() noexcept
{
    dryBuffer.clear();
    dcBlocker.reset();
    driveProcessor.reset();
    filterProcessor.reset();
    delayProcessor.reset();
    reverbProcessor.reset();
    wobbleProcessor.reset();
    widthProcessor.reset();
    safetyLimiter.reset();
}

void SignalChain::setTransportBpm (double bpm) noexcept
{
    delayProcessor.setTransportBpm (bpm);
    reverbProcessor.setTransportBpm (bpm);
}

bool SignalChain::process (juce::AudioBuffer<float>& buffer, const ParameterSnapshot& params, MeterState& meters) noexcept
{
    const int numChannels = juce::jmin (channels, buffer.getNumChannels());
    const int numSamples = buffer.getNumSamples();

    if (numChannels <= 0 || numSamples <= 0)
        return true;

    if (dryBuffer.getNumChannels() < numChannels || dryBuffer.getNumSamples() < numSamples)
        return false;

    for (int ch = 0; ch < numChannels; ++ch)
        dryBuffer.copyFrom (ch, 0, buffer, ch, 0, numSamples);

    driveProcessor.setBlockParameters (params);
    filterProcessor.setBlockParameters (params);
    delayProcessor.setBlockParameters (params);
    reverbProcessor.setBlockParameters (params);
    wobbleProcessor.setBlockParameters (params);
    widthProcessor.setBlockParameters (params);

    const float inputGain = SafeToolboxParameterUtilities::dbToLinear (params.inputGainDb);
    const float outputGain = SafeToolboxParameterUtilities::dbToLinear (params.outputGainDb);
    const float globalMix = juce::jlimit (0.0f, 1.0f, params.globalMix);

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float dryLeft = SafeToolboxSampleSanitizer::sanitiseSample (dryBuffer.getReadPointer (0)[sample]);
        const float dryRight = (numChannels > 1)
                                   ? SafeToolboxSampleSanitizer::sanitiseSample (dryBuffer.getReadPointer (1)[sample], dryLeft)
                                   : dryLeft;

        updatePeak (meters.inputPeak, juce::jmax (std::abs (dryLeft), std::abs (dryRight)));

        float left = dcBlocker.processSample (dryLeft * inputGain, 0);
        float right = dcBlocker.processSample (dryRight * inputGain, numChannels > 1 ? 1 : 0);

        buffer.getWritePointer (0)[sample] = left;
        if (numChannels > 1)
            buffer.getWritePointer (1)[sample] = right;
    }

    if (! params.bypass)
    {
        if (! params.driveBypass)
        {
            for (int sample = 0; sample < numSamples; ++sample)
            {
                buffer.getWritePointer (0)[sample] = driveProcessor.processSample (buffer.getWritePointer (0)[sample], 0);
                if (numChannels > 1)
                    buffer.getWritePointer (1)[sample] = driveProcessor.processSample (buffer.getWritePointer (1)[sample], 1);
            }
        }

        filterProcessor.process (buffer);
        delayProcessor.process (buffer);
        reverbProcessor.process (buffer);
        wobbleProcessor.process (buffer);
        widthProcessor.process (buffer);
    }

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float dryLeft = dryBuffer.getReadPointer (0)[sample];
        const float dryRight = (numChannels > 1) ? dryBuffer.getReadPointer (1)[sample] : dryLeft;

        float left = buffer.getWritePointer (0)[sample];
        float right = numChannels > 1 ? buffer.getWritePointer (1)[sample] : left;

        if (params.bypass)
        {
            left = dryLeft;
            right = dryRight;
        }

        if (globalMix < 0.999f)
        {
            left = juce::jmap (globalMix, dryLeft, left);
            right = juce::jmap (globalMix, dryRight, right);
        }

        left *= outputGain;
        right *= outputGain;

        safetyLimiter.processFrame (left, right, params, meters);

        left = SafeToolboxSampleSanitizer::sanitiseSample (left);
        right = SafeToolboxSampleSanitizer::sanitiseSample (right, left);

        buffer.getWritePointer (0)[sample] = left;
        if (numChannels > 1)
            buffer.getWritePointer (1)[sample] = right;

        updatePeak (meters.outputPeak, juce::jmax (std::abs (left), std::abs (right)));
    }

    return ! SafeToolboxSampleSanitizer::bufferContainsInvalidSamples (buffer);
}

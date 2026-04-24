#include "ReverbProcessor.h"

#include <cmath>

#include "../Parameters/ParameterUtilities.h"

namespace
{
    static float readLinear (const juce::AudioBuffer<float>& buffer, int channel, float delaySamples, int writePos) noexcept
    {
        const int size = buffer.getNumSamples();
        if (size <= 1)
            return 0.0f;

        float readPos = static_cast<float> (writePos) - delaySamples;
        while (readPos < 0.0f)
            readPos += (float) size;
        while (readPos >= (float) size)
            readPos -= (float) size;

        const float floored = std::floor (readPos);
        const int index1 = juce::jlimit (0, size - 1, static_cast<int> (floored));
        const int index2 = (index1 + 1) % size;
        const float frac = readPos - floored;
        const auto* data = buffer.getReadPointer (channel);
        return juce::jmap (frac, data[index1], data[index2]);
    }
}

void ReverbProcessor::prepare (double sampleRate, int maximumBlockSize, int numChannels) noexcept
{
    currentSampleRate = sampleRate;
    currentBlockSize = maximumBlockSize;
    currentChannels = numChannels;

    maxPreDelaySamples = juce::jmax (1, static_cast<int> (std::ceil (currentSampleRate * 0.25)));
    workingBuffer.setSize (juce::jlimit (1, 2, currentChannels), juce::jmax (1, maximumBlockSize), false, false, true);
    preDelayBuffer.setSize (juce::jlimit (1, 2, currentChannels), maxPreDelaySamples + juce::jmax (1, maximumBlockSize) + 4, false, false, true);
    preDelayBuffer.clear();
    reverb.setSampleRate (currentSampleRate);
    preDelaySmoothed.reset (currentSampleRate, 0.05);
    reset();
}

void ReverbProcessor::reset() noexcept
{
    preDelayBuffer.clear();
    workingBuffer.clear();
    writePos = 0;
    toneState = { 0.0f, 0.0f };
    preDelaySmoothed.setCurrentAndTargetValue (1.0f);
    reverb.reset();
}

void ReverbProcessor::setTransportBpm (double bpm) noexcept
{
    currentBpm = juce::jlimit (30.0, 300.0, bpm);
}

void ReverbProcessor::setBlockParameters (const ParameterSnapshot& blockParams) noexcept
{
    this->params = blockParams;
    preDelaySmoothed.setTargetValue (juce::jlimit (0.0f, static_cast<float> (maxPreDelaySamples - 4), blockParams.reverbPreDelayMs * 0.001f * static_cast<float> (currentSampleRate)));

    juce::Reverb::Parameters reverbParams;
    reverbParams.roomSize = juce::jlimit (0.0f, 1.0f, blockParams.reverbSize * (0.55f + blockParams.reverbDecay * 0.45f));
    reverbParams.damping = juce::jlimit (0.0f, 1.0f, blockParams.reverbDamping);
    reverbParams.wetLevel = 1.0f;
    reverbParams.dryLevel = 0.0f;
    reverbParams.width = juce::jlimit (0.0f, 1.0f, blockParams.reverbWidth);
    reverbParams.freezeMode = 0.0f;
    reverb.setParameters (reverbParams);
}

void ReverbProcessor::process (juce::AudioBuffer<float>& buffer) noexcept
{
    if (params.reverbBypass)
        return;

    juce::ScopedNoDenormals noDenormals;
    const int numChannels = juce::jmin (currentChannels, buffer.getNumChannels());
    const int numSamples = buffer.getNumSamples();
    if (numChannels <= 0 || numSamples <= 0)
        return;

    if (workingBuffer.getNumChannels() < numChannels || workingBuffer.getNumSamples() < numSamples)
        return;

    auto* outL = buffer.getWritePointer (0);
    auto* outR = numChannels > 1 ? buffer.getWritePointer (1) : nullptr;
    auto* preDelayL = preDelayBuffer.getWritePointer (0);
    auto* preDelayR = numChannels > 1 ? preDelayBuffer.getWritePointer (1) : nullptr;
    auto* workL = workingBuffer.getWritePointer (0);
    auto* workR = numChannels > 1 ? workingBuffer.getWritePointer (1) : nullptr;

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float delaySamples = preDelaySmoothed.getNextValue();
        for (int ch = 0; ch < numChannels; ++ch)
        {
            const float dry = ch == 0 ? outL[sample] : outR[sample];
            if (ch == 0)
            {
                preDelayL[writePos] = juce::jlimit (-4.0f, 4.0f, dry);
                workL[sample] = readLinear (preDelayBuffer, ch, delaySamples, writePos);
            }
            else
            {
                preDelayR[writePos] = juce::jlimit (-4.0f, 4.0f, dry);
                workR[sample] = readLinear (preDelayBuffer, ch, delaySamples, writePos);
            }
        }

        ++writePos;
        if (writePos >= preDelayBuffer.getNumSamples())
            writePos = 0;
    }

    if (numChannels > 1)
        reverb.processStereo (workL, workR, numSamples);
    else
        reverb.processMono (workL, numSamples);

    const float wetMix = juce::jlimit (0.0f, 0.45f, params.reverbMix);
    const float toneCutoff = juce::jlimit (120.0f, 20000.0f, params.reverbToneHz);
    const float toneCoeff = 1.0f - std::exp (-2.0f * juce::MathConstants<float>::pi * toneCutoff / static_cast<float> (currentSampleRate));

    for (int sample = 0; sample < numSamples; ++sample)
    {
        for (int ch = 0; ch < numChannels; ++ch)
        {
            const float dry = ch == 0 ? outL[sample] : outR[sample];
            float wet = workingBuffer.getReadPointer (ch)[sample];
            toneState[(size_t) ch] += toneCoeff * (wet - toneState[(size_t) ch]);
            wet = toneState[(size_t) ch];

            if (ch == 0)
                outL[sample] = juce::jmap (wetMix, dry, wet);
            else
                outR[sample] = juce::jmap (wetMix, dry, wet);
        }
    }

    if (std::abs (toneState[0]) < 1.0e-20f)
        toneState[0] = 0.0f;
    if (numChannels > 1 && std::abs (toneState[1]) < 1.0e-20f)
        toneState[1] = 0.0f;
}

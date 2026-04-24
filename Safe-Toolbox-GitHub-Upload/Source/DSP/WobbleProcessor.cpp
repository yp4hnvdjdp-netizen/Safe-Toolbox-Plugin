#include "WobbleProcessor.h"

#include <cmath>

namespace
{
    static float triangleWave (float phase) noexcept
    {
        const float wrapped = phase - std::floor (phase);
        return 4.0f * std::abs (wrapped - 0.5f) - 1.0f;
    }

    static float sineWave (float phase) noexcept
    {
        return std::sin (phase * juce::MathConstants<float>::twoPi);
    }

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

void WobbleProcessor::prepare (double sampleRate, int maximumBlockSize, int numChannels) noexcept
{
    currentSampleRate = sampleRate;
    currentBlockSize = maximumBlockSize;
    currentChannels = numChannels;

    maxDelaySamples = juce::jmax (1, static_cast<int> (std::ceil (currentSampleRate * 0.03)));
    delayBuffer.setSize (juce::jlimit (1, 2, currentChannels), maxDelaySamples + juce::jmax (1, maximumBlockSize) + 4, false, false, true);
    delayBuffer.clear();

    rateSmoothed.reset (currentSampleRate, 0.05);
    depthSmoothed.reset (currentSampleRate, 0.05);
    shapeSmoothed.reset (currentSampleRate, 0.05);
    stereoPhaseSmoothed.reset (currentSampleRate, 0.05);
    mixSmoothed.reset (currentSampleRate, 0.05);
    reset();
}

void WobbleProcessor::reset() noexcept
{
    delayBuffer.clear();
    writePos = 0;
    lfoPhase = 0.0f;
    noisePhase = 0.0f;
    rateSmoothed.setCurrentAndTargetValue (0.2f);
    depthSmoothed.setCurrentAndTargetValue (0.0f);
    shapeSmoothed.setCurrentAndTargetValue (0.5f);
    stereoPhaseSmoothed.setCurrentAndTargetValue (0.5f);
    mixSmoothed.setCurrentAndTargetValue (0.0f);
}

void WobbleProcessor::setBlockParameters (const ParameterSnapshot& blockParams) noexcept
{
    this->params = blockParams;
    rateSmoothed.setTargetValue (juce::jlimit (0.05f, 8.0f, blockParams.wobbleRateHz));
    depthSmoothed.setTargetValue (juce::jlimit (0.0f, 12.0f, blockParams.wobbleDepthMs));
    shapeSmoothed.setTargetValue (juce::jlimit (0.0f, 1.0f, blockParams.wobbleShape));
    stereoPhaseSmoothed.setTargetValue (juce::jlimit (0.0f, 1.0f, blockParams.wobbleStereoPhase));
    mixSmoothed.setTargetValue (juce::jlimit (0.0f, 1.0f, blockParams.wobbleMix));
}

void WobbleProcessor::process (juce::AudioBuffer<float>& buffer) noexcept
{
    if (params.wobbleBypass)
        return;

    juce::ScopedNoDenormals noDenormals;
    const int numChannels = juce::jmin (currentChannels, buffer.getNumChannels());
    const int numSamples = buffer.getNumSamples();
    if (numChannels <= 0 || numSamples <= 0)
        return;

    const int usableChannels = juce::jmin (2, numChannels);
    auto* outL = buffer.getWritePointer (0);
    auto* outR = numChannels > 1 ? buffer.getWritePointer (1) : nullptr;
    auto* delayL = delayBuffer.getWritePointer (0);
    auto* delayR = numChannels > 1 ? delayBuffer.getWritePointer (1) : nullptr;

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float rate = rateSmoothed.getNextValue();
        const float depthMs = depthSmoothed.getNextValue();
        const float shape = shapeSmoothed.getNextValue();
        const float stereoPhase = stereoPhaseSmoothed.getNextValue();
        const float mix = mixSmoothed.getNextValue();

        const float phaseIncrement = rate / static_cast<float> (currentSampleRate);
        lfoPhase += phaseIncrement;
        if (lfoPhase >= 1.0f)
            lfoPhase -= 1.0f;

        const float noiseIncrement = 573.0f / static_cast<float> (currentSampleRate);
        noisePhase += noiseIncrement;
        if (noisePhase >= 1.0f)
            noisePhase -= 1.0f;

        const float baseWave = juce::jmap (shape, sineWave (lfoPhase), triangleWave (lfoPhase));
        const float randomDrift = 0.15f * std::sin (noisePhase * juce::MathConstants<float>::twoPi * 31.0f);
        const float modulation = juce::jlimit (-1.0f, 1.0f, baseWave + randomDrift * (params.wobbleMode == 3 ? 0.9f : 0.4f));

        for (int ch = 0; ch < usableChannels; ++ch)
        {
            const float dry = ch == 0 ? outL[sample] : outR[sample];
            if (ch == 0)
                delayL[writePos] = juce::jlimit (-4.0f, 4.0f, dry);
            else
                delayR[writePos] = juce::jlimit (-4.0f, 4.0f, dry);

            const float channelPhaseOffset = (ch == 0 ? 0.0f : stereoPhase * 0.5f);
            const float channelWave = juce::jmap (shape, sineWave (lfoPhase + channelPhaseOffset), triangleWave (lfoPhase + channelPhaseOffset));
            const float wobble = juce::jlimit (-1.0f, 1.0f, channelWave + modulation * 0.35f);
            const float baseDelay = 3.0f + depthMs * 0.6f + (params.wobbleMode == 2 ? 1.5f : 0.0f);
            const float delaySamples = juce::jlimit (1.0f, static_cast<float> (maxDelaySamples - 4),
                                                      (baseDelay + wobble * depthMs * 0.8f) * 0.001f * static_cast<float> (currentSampleRate));

            const float wet = readLinear (delayBuffer, ch, delaySamples, writePos);
            if (ch == 0)
                outL[sample] = juce::jmap (mix, dry, wet);
            else
                outR[sample] = juce::jmap (mix, dry, wet);
        }

        ++writePos;
        if (writePos >= delayBuffer.getNumSamples())
            writePos = 0;
    }

    if (std::abs (lfoPhase) < 1.0e-20f)
        lfoPhase = 0.0f;
    if (std::abs (noisePhase) < 1.0e-20f)
        noisePhase = 0.0f;
}

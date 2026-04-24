#include "DelayProcessor.h"

#include <cmath>

#include "../Parameters/ParameterUtilities.h"

namespace
{
    static float linearDelayRead (const juce::AudioBuffer<float>& buffer, int channel, float delaySamples, int writePos) noexcept
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

void DelayProcessor::prepare (double sampleRate, int maximumBlockSize, int numChannels) noexcept
{
    currentSampleRate = sampleRate;
    currentBlockSize = maximumBlockSize;
    currentChannels = numChannels;

    maxDelaySamples = juce::jmax (1, static_cast<int> (std::ceil (currentSampleRate * 2.5)));
    delayBuffer.setSize (juce::jlimit (1, 2, currentChannels), maxDelaySamples + juce::jmax (1, maximumBlockSize) + 4, false, false, true);
    delayBuffer.clear();

    delaySamplesSmoothed.reset (currentSampleRate, 0.05);
    feedbackSmoothed.reset (currentSampleRate, 0.05);
    mixSmoothed.reset (currentSampleRate, 0.05);
    dampingSmoothed.reset (currentSampleRate, 0.05);
    offsetSmoothed.reset (currentSampleRate, 0.05);
    reset();
}

void DelayProcessor::reset() noexcept
{
    delayBuffer.clear();
    writePos = 0;
    feedbackLowpassState = { 0.0f, 0.0f };
    delaySamplesSmoothed.setCurrentAndTargetValue (1.0f);
    feedbackSmoothed.setCurrentAndTargetValue (0.0f);
    mixSmoothed.setCurrentAndTargetValue (0.0f);
    dampingSmoothed.setCurrentAndTargetValue (0.0f);
    offsetSmoothed.setCurrentAndTargetValue (0.0f);
}

void DelayProcessor::setTransportBpm (double bpm) noexcept
{
    currentBpm = juce::jlimit (30.0, 300.0, bpm);
}

void DelayProcessor::setBlockParameters (const ParameterSnapshot& blockParams) noexcept
{
    this->params = blockParams;

    float delayMs = blockParams.delayTimeSeconds * 1000.0f;
    if (blockParams.delaySync > 0)
        delayMs = SafeToolboxParameterUtilities::syncDelayMsToTempo (delayMs, blockParams.delaySync, static_cast<float> (currentBpm));

    delaySamplesSmoothed.setTargetValue (juce::jlimit (1.0f, static_cast<float> (maxDelaySamples - 4), delayMs * 0.001f * static_cast<float> (currentSampleRate)));
    feedbackSmoothed.setTargetValue (juce::jlimit (0.0f, 0.65f, blockParams.delayFeedback));
    mixSmoothed.setTargetValue (juce::jlimit (0.0f, 0.5f, blockParams.delayMix));
    dampingSmoothed.setTargetValue (juce::jlimit (0.0f, 1.0f, blockParams.delayDamping));
    offsetSmoothed.setTargetValue (juce::jlimit (0.0f, 50.0f, blockParams.delayStereoOffsetMs));
}

void DelayProcessor::process (juce::AudioBuffer<float>& buffer) noexcept
{
    if (params.delayBypass)
        return;

    juce::ScopedNoDenormals noDenormals;
    const int numChannels = juce::jmin (currentChannels, buffer.getNumChannels());
    const int numSamples = buffer.getNumSamples();
    if (numChannels <= 0 || numSamples <= 0)
        return;

    const bool pingPong = params.delayMode == 1;
    const float feedbackToneHz = juce::jlimit (80.0f, 20000.0f, params.delayFeedbackToneHz);
    const float dampingBase = juce::jlimit (0.0f, 1.0f, dampingSmoothed.getTargetValue());
    const float filterCoeff = juce::jlimit (0.0001f, 1.0f,
                                            1.0f - std::exp (-2.0f * juce::MathConstants<float>::pi * juce::jmax (80.0f, feedbackToneHz * (1.0f - dampingBase * 0.9f) + 80.0f) / static_cast<float> (currentSampleRate)));

    auto* outL = buffer.getWritePointer (0);
    auto* outR = numChannels > 1 ? buffer.getWritePointer (1) : nullptr;
    auto* delayL = delayBuffer.getWritePointer (0);
    auto* delayR = numChannels > 1 ? delayBuffer.getWritePointer (1) : nullptr;

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float delaySamples = delaySamplesSmoothed.getNextValue();
        const float feedback = feedbackSmoothed.getNextValue();
        const float mix = mixSmoothed.getNextValue();
        const float offsetSamples = offsetSmoothed.getNextValue() * 0.001f * static_cast<float> (currentSampleRate);

        const float dryL = outL[sample];
        const float dryR = numChannels > 1 ? outR[sample] : dryL;

        const float readL = linearDelayRead (delayBuffer, 0, delaySamples, writePos);
        const float readR = numChannels > 1 ? linearDelayRead (delayBuffer, 1, delaySamples + offsetSamples, writePos)
                                           : readL;

        feedbackLowpassState[0] += filterCoeff * (readL - feedbackLowpassState[0]);
        feedbackLowpassState[1] += filterCoeff * (readR - feedbackLowpassState[1]);

        const float fbL = juce::jlimit (-1.5f, 1.5f, pingPong ? feedbackLowpassState[1] : feedbackLowpassState[0]);
        const float fbR = juce::jlimit (-1.5f, 1.5f, pingPong ? feedbackLowpassState[0] : feedbackLowpassState[1]);

        const float writeL = dryL + fbL * feedback;
        const float writeR = dryR + fbR * feedback;

        delayL[writePos] = juce::jlimit (-4.0f, 4.0f, writeL);
        if (numChannels > 1)
            delayR[writePos] = juce::jlimit (-4.0f, 4.0f, writeR);

        outL[sample] = juce::jmap (mix, dryL, readL);
        if (numChannels > 1)
            outR[sample] = juce::jmap (mix, dryR, readR);

        ++writePos;
        if (writePos >= delayBuffer.getNumSamples())
            writePos = 0;
    }

    if (std::abs (feedbackLowpassState[0]) < 1.0e-20f)
        feedbackLowpassState[0] = 0.0f;
    if (numChannels > 1 && std::abs (feedbackLowpassState[1]) < 1.0e-20f)
        feedbackLowpassState[1] = 0.0f;
}

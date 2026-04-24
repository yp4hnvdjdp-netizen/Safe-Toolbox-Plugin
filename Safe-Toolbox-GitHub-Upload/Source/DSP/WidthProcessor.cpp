#include "WidthProcessor.h"

#include <cmath>

#include "../Parameters/ParameterUtilities.h"

void WidthProcessor::prepare (double sampleRate, int maximumBlockSize, int numChannels) noexcept
{
    currentSampleRate = sampleRate;
    currentBlockSize = maximumBlockSize;
    currentChannels = numChannels;
    juce::ignoreUnused (currentBlockSize);
    reset();
}

void WidthProcessor::reset() noexcept
{
    lowState = { 0.0f, 0.0f };
    highState = { 0.0f, 0.0f };
    sideLowState = { 0.0f, 0.0f };
    sideHighState = { 0.0f, 0.0f };
}

void WidthProcessor::setBlockParameters (const ParameterSnapshot& blockParams) noexcept
{
    this->params = blockParams;
}

void WidthProcessor::process (juce::AudioBuffer<float>& buffer) noexcept
{
    if (params.widthBypass)
        return;

    const int numChannels = juce::jmin (currentChannels, buffer.getNumChannels());
    const int numSamples = buffer.getNumSamples();
    if (numChannels <= 0 || numSamples <= 0)
        return;

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float left = buffer.getWritePointer (0)[sample];
        const float right = numChannels > 1 ? buffer.getWritePointer (1)[sample] : left;
        const float mid = 0.5f * (left + right);
        const float side = 0.5f * (left - right);

        const float lowCut = juce::jlimit (20.0f, 1000.0f, params.widthLowCutHz);
        const float highCut = juce::jlimit (1000.0f, 20000.0f, params.widthHighCutHz);
        const float lowMid = lowpass (mid, lowState[0], lowCut);
        const float lowSide = lowpass (side, sideLowState[0], lowCut);
        const float highMid = mid - lowMid;
        const float highSide = side - lowSide;
        const float monoSafeSide = params.widthMonoBelow ? 0.0f : lowSide;
        const float wideSide = lowpass (highSide, sideHighState[0], highCut) * juce::jlimit (0.0f, 1.5f, params.width);
        const float finalSide = monoSafeSide + wideSide;
        const float finalMid = lowMid + highMid;

        float outL = finalMid + finalSide;
        float outR = finalMid - finalSide;

        const float balance = juce::jlimit (-1.0f, 1.0f, params.balance);
        const float leftGain = balance <= 0.0f ? 1.0f : 1.0f - balance;
        const float rightGain = balance >= 0.0f ? 1.0f : 1.0f + balance;
        outL *= leftGain;
        outR *= rightGain;

        buffer.getWritePointer (0)[sample] = juce::jlimit (-4.0f, 4.0f, outL);
        if (numChannels > 1)
            buffer.getWritePointer (1)[sample] = juce::jlimit (-4.0f, 4.0f, outR);
    }
}

float WidthProcessor::lowpass (float input, float& state, float cutoffHz) noexcept
{
    const float coeff = 1.0f - std::exp (-2.0f * juce::MathConstants<float>::pi * cutoffHz / static_cast<float> (currentSampleRate));
    state += coeff * (input - state);
    return state;
}

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <array>

#include "../Parameters/ParameterSnapshot.h"

class WobbleProcessor final
{
public:
    void prepare (double sampleRate, int maximumBlockSize, int numChannels) noexcept;
    void reset() noexcept;
    void setBlockParameters (const ParameterSnapshot& params) noexcept;
    void process (juce::AudioBuffer<float>& buffer) noexcept;

private:
    float readDelay (int channel, float delaySamples) noexcept;

    double currentSampleRate = 44100.0;
    int currentChannels = 2;
    int currentBlockSize = 0;
    int maxDelaySamples = 0;
    int writePos = 0;
    float lfoPhase = 0.0f;
    float noisePhase = 0.0f;
    ParameterSnapshot params;
    juce::AudioBuffer<float> delayBuffer;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> rateSmoothed;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> depthSmoothed;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> shapeSmoothed;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> stereoPhaseSmoothed;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> mixSmoothed;
};

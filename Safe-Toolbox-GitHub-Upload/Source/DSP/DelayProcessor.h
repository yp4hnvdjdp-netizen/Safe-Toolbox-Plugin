#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <array>

#include "../Parameters/ParameterSnapshot.h"

class DelayProcessor final
{
public:
    void prepare (double sampleRate, int maximumBlockSize, int numChannels) noexcept;
    void reset() noexcept;
    void setTransportBpm (double bpm) noexcept;
    void setBlockParameters (const ParameterSnapshot& params) noexcept;
    void process (juce::AudioBuffer<float>& buffer) noexcept;

private:
    float readSample (int channel, float delaySamples) noexcept;
    void writeSample (int channel, float sample) noexcept;

    double currentSampleRate = 44100.0;
    int currentChannels = 2;
    int currentBlockSize = 0;
    double currentBpm = 120.0;
    int maxDelaySamples = 0;
    int writePos = 0;
    ParameterSnapshot params;
    juce::AudioBuffer<float> delayBuffer;
    std::array<float, 2> feedbackLowpassState {};
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> delaySamplesSmoothed;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> feedbackSmoothed;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> mixSmoothed;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> dampingSmoothed;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> offsetSmoothed;
};

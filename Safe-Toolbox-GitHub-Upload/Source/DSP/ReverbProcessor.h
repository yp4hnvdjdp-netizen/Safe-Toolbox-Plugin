#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <array>

#include "../Parameters/ParameterSnapshot.h"

class ReverbProcessor final
{
public:
    void prepare (double sampleRate, int maximumBlockSize, int numChannels) noexcept;
    void reset() noexcept;
    void setTransportBpm (double bpm) noexcept;
    void setBlockParameters (const ParameterSnapshot& params) noexcept;
    void process (juce::AudioBuffer<float>& buffer) noexcept;

private:
    float readFromPreDelay (int channel, float delaySamples) noexcept;

    double currentSampleRate = 44100.0;
    int currentChannels = 2;
    int currentBlockSize = 0;
    double currentBpm = 120.0;
    int maxPreDelaySamples = 0;
    int writePos = 0;
    ParameterSnapshot params;
    juce::Reverb reverb;
    juce::AudioBuffer<float> workingBuffer;
    juce::AudioBuffer<float> preDelayBuffer;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> preDelaySmoothed;
    std::array<float, 2> toneState {};
};

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <array>

#include "../Parameters/ParameterSnapshot.h"

class WidthProcessor final
{
public:
    void prepare (double sampleRate, int maximumBlockSize, int numChannels) noexcept;
    void reset() noexcept;
    void setBlockParameters (const ParameterSnapshot& params) noexcept;
    void process (juce::AudioBuffer<float>& buffer) noexcept;

private:
    float lowpass (float input, float& state, float cutoffHz) noexcept;

    double currentSampleRate = 44100.0;
    int currentChannels = 2;
    int currentBlockSize = 0;
    ParameterSnapshot params;
    std::array<float, 2> lowState {};
    std::array<float, 2> highState {};
    std::array<float, 2> sideLowState {};
    std::array<float, 2> sideHighState {};
};

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <array>

#include "../Parameters/ParameterSnapshot.h"

class FilterProcessor final
{
public:
    void prepare (double sampleRate, int maximumBlockSize, int numChannels) noexcept;
    void reset() noexcept;
    void setBlockParameters (const ParameterSnapshot& params) noexcept;
    void process (juce::AudioBuffer<float>& buffer) noexcept;

    struct Biquad
    {
        float b0 = 1.0f, b1 = 0.0f, b2 = 0.0f;
        float a1 = 0.0f, a2 = 0.0f;
        float z1 = 0.0f, z2 = 0.0f;

        void reset() noexcept { z1 = z2 = 0.0f; }

        float process (float x) noexcept
        {
            const float y = b0 * x + z1;
            z1 = b1 * x - a1 * y + z2;
            z2 = b2 * x - a2 * y;
            return y;
        }
    };

private:
    void updateCoefficients() noexcept;

    double currentSampleRate = 44100.0;
    int currentChannels = 2;
    int currentBlockSize = 0;
    ParameterSnapshot params;
    std::array<std::array<Biquad, 2>, 2> stages {};
    int activeStages = 1;
    bool bypass = false;
};

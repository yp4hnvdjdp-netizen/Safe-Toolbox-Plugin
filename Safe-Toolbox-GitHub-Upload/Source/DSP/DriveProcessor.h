#pragma once

#include <array>
#include <juce_audio_basics/juce_audio_basics.h>

#include "../Parameters/ParameterSnapshot.h"

class DriveProcessor final
{
public:
    void prepare (double sampleRate, int numChannels) noexcept;
    void reset() noexcept;
    void setBlockParameters (const ParameterSnapshot& params) noexcept;
    float processSample (float input, int channel) noexcept;

private:
    struct OnePoleLowPass
    {
        void prepare (double newSampleRate) noexcept { sampleRate = newSampleRate; }
        void reset() noexcept { z = 0.0f; }
        void setCutoffHz (float hz) noexcept;
        float process (float input) noexcept;

        double sampleRate = 44100.0;
        float coefficient = 1.0f;
        float z = 0.0f;
    };

    std::array<OnePoleLowPass, 2> toneFilters {};
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> driveAmount;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> driveMix;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> driveBias;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> driveOutput;
    double sampleRate = 44100.0;
    int channels = 2;
};

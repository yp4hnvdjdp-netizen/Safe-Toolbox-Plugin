#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

#include "../Parameters/ParameterSnapshot.h"
#include "../Parameters/ParameterUtilities.h"
#include "MeterState.h"

class SafetyLimiter final
{
public:
    void prepare (double sampleRate, int numChannels) noexcept;
    void reset() noexcept;
    void processFrame (float& left, float& right, const ParameterSnapshot& params, MeterState& meters) noexcept;

private:
    double sampleRate = 44100.0;
    int channels = 2;
    float gain = 1.0f;
};

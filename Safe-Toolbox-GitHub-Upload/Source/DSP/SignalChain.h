#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

#include "../Parameters/ParameterSnapshot.h"
#include "DcBlocker.h"
#include "DriveProcessor.h"
#include "DelayProcessor.h"
#include "FilterProcessor.h"
#include "MeterState.h"
#include "ReverbProcessor.h"
#include "SafetyLimiter.h"
#include "WidthProcessor.h"
#include "WobbleProcessor.h"

class SignalChain final
{
public:
    void prepare (double sampleRate, int maximumBlockSize, int numChannels) noexcept;
    void reset() noexcept;
    void setTransportBpm (double bpm) noexcept;
    bool process (juce::AudioBuffer<float>& buffer, const ParameterSnapshot& params, MeterState& meters) noexcept;

private:
    double sampleRate = 44100.0;
    int channels = 2;
    juce::AudioBuffer<float> dryBuffer;
    DcBlocker dcBlocker;
    DriveProcessor driveProcessor;
    FilterProcessor filterProcessor;
    DelayProcessor delayProcessor;
    ReverbProcessor reverbProcessor;
    WobbleProcessor wobbleProcessor;
    WidthProcessor widthProcessor;
    SafetyLimiter safetyLimiter;
};

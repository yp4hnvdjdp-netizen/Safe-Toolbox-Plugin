#include "DriveProcessor.h"

#include <cmath>

#include "../Parameters/ParameterUtilities.h"

namespace
{
    inline float fastTanh (float value) noexcept
    {
        return std::tanh (juce::jlimit (-12.0f, 12.0f, value));
    }
}

void DriveProcessor::OnePoleLowPass::setCutoffHz (float hz) noexcept
{
    hz = juce::jlimit (20.0f, 20000.0f, hz);
    const auto x = std::exp (-2.0f * juce::MathConstants<float>::pi * hz / static_cast<float> (sampleRate));
    coefficient = 1.0f - x;
}

float DriveProcessor::OnePoleLowPass::process (float input) noexcept
{
    z += coefficient * (input - z);
    return z;
}

void DriveProcessor::prepare (double newSampleRate, int numChannels) noexcept
{
    sampleRate = juce::jmax (newSampleRate, 1.0);
    channels = juce::jlimit (1, 2, numChannels);

    for (auto& toneFilter : toneFilters)
        toneFilter.prepare (sampleRate);

    driveAmount.reset (sampleRate, 0.03);
    driveMix.reset (sampleRate, 0.05);
    driveBias.reset (sampleRate, 0.05);
    driveOutput.reset (sampleRate, 0.05);
    reset();
}

void DriveProcessor::reset() noexcept
{
    for (auto& toneFilter : toneFilters)
        toneFilter.reset();

    driveAmount.setCurrentAndTargetValue (0.0f);
    driveMix.setCurrentAndTargetValue (0.0f);
    driveBias.setCurrentAndTargetValue (0.0f);
    driveOutput.setCurrentAndTargetValue (0.0f);
}

void DriveProcessor::setBlockParameters (const ParameterSnapshot& blockParams) noexcept
{
    driveAmount.setTargetValue (juce::jlimit (0.0f, 1.0f, blockParams.driveAmount));
    driveMix.setTargetValue (juce::jlimit (0.0f, 1.0f, blockParams.driveMix));
    driveBias.setTargetValue (juce::jlimit (-1.0f, 1.0f, blockParams.driveBias));
    driveOutput.setTargetValue (juce::jlimit (-24.0f, 24.0f, blockParams.driveOutputDb));

    for (auto& toneFilter : toneFilters)
        toneFilter.setCutoffHz (blockParams.driveToneHz);
}

float DriveProcessor::processSample (float input, int channel) noexcept
{
    const auto ch = juce::jlimit (0, channels - 1, channel);
    auto& toneFilter = toneFilters[(size_t) ch];

    const float amount = driveAmount.getNextValue();
    const float mix = driveMix.getNextValue();
    const float bias = driveBias.getNextValue();

    const float toned = toneFilter.process (input);
    const float shaped = fastTanh ((toned + bias * 0.25f) * (1.0f + amount * 14.0f));
    const float outputGain = SafeToolboxParameterUtilities::dbToLinear (driveOutput.getNextValue());

    return juce::jmap (mix, input, shaped) * outputGain;
}

#include "SafetyLimiter.h"

#include <cmath>

namespace
{
    inline float applySoftClip (float input, float ceiling, float amount) noexcept
    {
        if (amount <= 0.0f)
            return input;

        const float clipped = ceiling * std::tanh (input / juce::jmax (0.0001f, ceiling));
        return juce::jmap (juce::jlimit (0.0f, 1.0f, amount), input, clipped);
    }
}

void SafetyLimiter::prepare (double newSampleRate, int numChannels) noexcept
{
    sampleRate = juce::jmax (newSampleRate, 1.0);
    channels = juce::jlimit (1, 2, numChannels);
    reset();
}

void SafetyLimiter::reset() noexcept
{
    gain = 1.0f;
}

void SafetyLimiter::processFrame (float& left, float& right, const ParameterSnapshot& params, MeterState& meters) noexcept
{
    juce::ignoreUnused (channels);

    const float ceiling = SafeToolboxParameterUtilities::dbToLinear (params.safetyCeilingDb);
    const float threshold = juce::jlimit (0.00001f, ceiling, SafeToolboxParameterUtilities::dbToLinear (params.limiterThresholdDb));
    const float maxAbs = juce::jmax (std::abs (left), std::abs (right));

    float targetGain = 1.0f;
    if (maxAbs > threshold)
        targetGain = threshold / juce::jmax (0.00001f, maxAbs);

    if (targetGain < gain)
    {
        gain = targetGain;
    }
    else
    {
        const float releaseMs = juce::jlimit (5.0f, 500.0f, params.limiterReleaseMs);
        const float coeff = std::exp (-1.0f / juce::jmax (1.0f, releaseMs * 0.001f * static_cast<float> (sampleRate)));
        gain = juce::jlimit (targetGain, 1.0f, gain + (1.0f - gain) * (1.0f - coeff));
    }

    const float appliedGain = gain;
    left *= appliedGain;
    right *= appliedGain;

    left = applySoftClip (left, ceiling, params.softClip);
    right = applySoftClip (right, ceiling, params.softClip);

    left = juce::jlimit (-ceiling, ceiling, left);
    right = juce::jlimit (-ceiling, ceiling, right);

    const float gainReductionDb = juce::jmax (0.0f, -SafeToolboxParameterUtilities::linearToDb (juce::jlimit (0.00001f, 1.0f, gain)));
    if (gainReductionDb > meters.gainReductionDb.load (std::memory_order_relaxed))
        meters.gainReductionDb.store (gainReductionDb, std::memory_order_relaxed);

    if (gainReductionDb > 0.0f)
        meters.clipWarning.store (1.0f, std::memory_order_relaxed);
}

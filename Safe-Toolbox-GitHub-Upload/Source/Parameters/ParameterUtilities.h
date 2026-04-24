#pragma once

#include <cmath>
#include <juce_audio_basics/juce_audio_basics.h>

namespace SafeToolboxParameterUtilities
{
    inline float sanitiseFloat (float value, float fallback = 0.0f) noexcept
    {
        return std::isfinite (value) ? value : fallback;
    }

    inline float clampFloat (float value, float low, float high) noexcept
    {
        return juce::jlimit (low, high, sanitiseFloat (value, low));
    }

    inline float dbToLinear (float db) noexcept
    {
        return juce::Decibels::decibelsToGain (db, -120.0f);
    }

    inline float linearToDb (float linear) noexcept
    {
        return juce::Decibels::gainToDecibels (juce::jmax (0.00001f, linear), -120.0f);
    }

    inline float syncDelayMsToTempo (float delayMs, int syncIndex, float bpm) noexcept
    {
        const float beatMs = 60000.0f / juce::jmax (30.0f, bpm);

        switch (syncIndex)
        {
            case 1: return beatMs * 1.0f;
            case 2: return beatMs * 0.5f;
            case 3: return beatMs * 0.75f;
            case 4: return beatMs * 0.25f;
            case 5: return beatMs * 1.5f;
            default: return delayMs;
        }
    }
}

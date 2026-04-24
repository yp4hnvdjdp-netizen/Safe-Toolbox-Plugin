#pragma once

#include <cmath>
#include <juce_audio_basics/juce_audio_basics.h>

namespace SafeToolboxSampleSanitizer
{
    inline float sanitiseSample (float value, float fallback = 0.0f) noexcept
    {
        if (! std::isfinite (value))
            return fallback;

        return juce::jlimit (-4.0f, 4.0f, value);
    }

    inline bool bufferContainsInvalidSamples (const juce::AudioBuffer<float>& buffer) noexcept
    {
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            const auto* ptr = buffer.getReadPointer (ch);

            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                if (! std::isfinite (ptr[sample]) || std::abs (ptr[sample]) > 4.0f)
                    return true;
            }
        }

        return false;
    }
}

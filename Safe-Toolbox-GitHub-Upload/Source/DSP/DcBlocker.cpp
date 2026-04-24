#include "DcBlocker.h"

#include <juce_core/juce_core.h>

void DcBlocker::prepare (double sampleRate, int numChannels) noexcept
{
    channels = juce::jlimit (1, 2, numChannels);
    const auto sr = juce::jmax (sampleRate, 1.0);
    r = std::exp (-2.0f * juce::MathConstants<float>::pi * 20.0f / static_cast<float> (sr));
    reset();
}

void DcBlocker::reset() noexcept
{
    for (auto& state : states)
    {
        state.x1 = 0.0f;
        state.y1 = 0.0f;
    }
}

float DcBlocker::processSample (float input, int channel) noexcept
{
    const auto ch = juce::jlimit (0, channels - 1, channel);
    auto& state = states[(size_t) ch];
    const float output = input - state.x1 + r * state.y1;
    state.x1 = input;
    state.y1 = output;
    return output;
}

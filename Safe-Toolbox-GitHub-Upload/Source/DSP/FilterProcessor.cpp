#include "FilterProcessor.h"

#include <cmath>

#include "../Parameters/ParameterUtilities.h"

namespace
{
    enum class Mode
    {
        Off = 0,
        Lowpass = 1,
        Highpass = 2,
        Bandpass = 3,
        Notch = 4
    };
}

static void makeCoefficients (FilterProcessor::Biquad& filter,
                              Mode mode,
                              double sampleRate,
                              float cutoff,
                              float q) noexcept
{
    cutoff = juce::jlimit (20.0f, 20000.0f, cutoff);
    q = juce::jlimit (0.2f, 12.0f, q);

    const auto w0 = 2.0 * juce::MathConstants<double>::pi * static_cast<double> (cutoff) / juce::jmax (sampleRate, 1.0);
    const auto cosw0 = std::cos (w0);
    const auto sinw0 = std::sin (w0);
    const auto alpha = sinw0 / (2.0 * juce::jmax (0.001, static_cast<double> (q)));
    double b0 = 1.0, b1 = 0.0, b2 = 0.0, a0 = 1.0, a1 = 0.0, a2 = 0.0;

    switch (mode)
    {
        case Mode::Lowpass:
            b0 = (1.0 - cosw0) * 0.5;
            b1 = 1.0 - cosw0;
            b2 = (1.0 - cosw0) * 0.5;
            a0 = 1.0 + alpha;
            a1 = -2.0 * cosw0;
            a2 = 1.0 - alpha;
            break;
        case Mode::Highpass:
            b0 = (1.0 + cosw0) * 0.5;
            b1 = -(1.0 + cosw0);
            b2 = (1.0 + cosw0) * 0.5;
            a0 = 1.0 + alpha;
            a1 = -2.0 * cosw0;
            a2 = 1.0 - alpha;
            break;
        case Mode::Bandpass:
            b0 = alpha;
            b1 = 0.0;
            b2 = -alpha;
            a0 = 1.0 + alpha;
            a1 = -2.0 * cosw0;
            a2 = 1.0 - alpha;
            break;
        case Mode::Notch:
            b0 = 1.0;
            b1 = -2.0 * cosw0;
            b2 = 1.0;
            a0 = 1.0 + alpha;
            a1 = -2.0 * cosw0;
            a2 = 1.0 - alpha;
            break;
        case Mode::Off:
            filter = {};
            filter.b0 = 1.0f;
            return;
    }

    const auto invA0 = 1.0 / juce::jmax (0.000001, a0);
    filter.b0 = static_cast<float> (b0 * invA0);
    filter.b1 = static_cast<float> (b1 * invA0);
    filter.b2 = static_cast<float> (b2 * invA0);
    filter.a1 = static_cast<float> (a1 * invA0);
    filter.a2 = static_cast<float> (a2 * invA0);
}

void FilterProcessor::prepare (double sampleRate, int maximumBlockSize, int numChannels) noexcept
{
    currentSampleRate = sampleRate;
    currentBlockSize = maximumBlockSize;
    currentChannels = numChannels;
    juce::ignoreUnused (currentBlockSize);
    reset();
}

void FilterProcessor::reset() noexcept
{
    for (auto& channel : stages)
        for (auto& stage : channel)
            stage.reset();
}

void FilterProcessor::setBlockParameters (const ParameterSnapshot& blockParams) noexcept
{
    this->params = blockParams;
    bypass = blockParams.filterBypass;
    activeStages = blockParams.filterSlope > 0 ? 2 : 1;
    updateCoefficients();
}

void FilterProcessor::process (juce::AudioBuffer<float>& buffer) noexcept
{
    if (bypass)
        return;

    const int numChannels = juce::jmin (currentChannels, buffer.getNumChannels());
    const int numSamples = buffer.getNumSamples();

    if (numChannels <= 0 || numSamples <= 0)
        return;

    for (int sample = 0; sample < numSamples; ++sample)
    {
        for (int ch = 0; ch < numChannels; ++ch)
        {
            float x = buffer.getWritePointer (ch)[sample];
            if (params.filterDrive > 0.0f)
                x = std::tanh (x * (1.0f + params.filterDrive * 3.0f));

            for (int stage = 0; stage < activeStages; ++stage)
                x = stages[(size_t) ch][(size_t) stage].process (x);

            buffer.getWritePointer (ch)[sample] = x;
        }
    }
}

void FilterProcessor::updateCoefficients() noexcept
{
    const auto type = static_cast<Mode> (juce::jlimit (0, 4, params.filterType));
    const auto cutoff = params.filterCutoffHz;
    const auto q = params.filterResonance;

    for (auto& channel : stages)
    {
        makeCoefficients (channel[0], type, currentSampleRate, cutoff, q);
        makeCoefficients (channel[1], type, currentSampleRate, cutoff, q);
    }
}

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <memory>
#include <array>
#include "../../SafeToolboxProcessor.h"

class MeterComponent final : public juce::Component
{
public:
    enum class MeterKind
    {
        Input,
        Output,
        Reduction
    };

    MeterComponent (SafeToolboxAudioProcessor& audioProcessorToUse, MeterKind kindToUse);

    void paint (juce::Graphics& g) override;

private:
    SafeToolboxAudioProcessor& audioProcessor;
    MeterKind meterKind;
    std::unique_ptr<juce::Drawable> tickOverlay;
    std::unique_ptr<juce::Drawable> accentOverlay;
    float smoothedLevel = 0.0f;
    float peakHoldLevel = 0.0f;
    float clipFlash = 0.0f;
    double lastPaintMs = 0.0;
};

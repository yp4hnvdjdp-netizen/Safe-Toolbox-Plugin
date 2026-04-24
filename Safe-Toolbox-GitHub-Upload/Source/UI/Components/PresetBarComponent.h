#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "../../SafeToolboxProcessor.h"

class PresetBarComponent final : public juce::Component
{
public:
    explicit PresetBarComponent (SafeToolboxAudioProcessor&);

    void refreshPresets();
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    SafeToolboxAudioProcessor& audioProcessor;
    juce::ComboBox presetBox;
    juce::TextButton aButton { "A" };
    juce::TextButton bButton { "B" };
    juce::TextButton copyButton { "Copy" };
    juce::TextButton pasteButton { "Paste" };
    juce::TextButton compareButton { "Compare" };
};

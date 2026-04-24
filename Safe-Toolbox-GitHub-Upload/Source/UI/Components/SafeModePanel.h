#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <memory>
#include "../../SafeToolboxProcessor.h"

class SafeModePanel final : public juce::Component
{
public:
    explicit SafeModePanel (SafeToolboxAudioProcessor&);
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    SafeToolboxAudioProcessor& audioProcessor;
    std::unique_ptr<juce::Drawable> badgeIcon;
    std::unique_ptr<juce::Drawable> ledIcon;
};

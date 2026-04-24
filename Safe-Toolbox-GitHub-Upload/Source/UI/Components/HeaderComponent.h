#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class HeaderComponent final : public juce::Component
{
public:
    HeaderComponent();

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    std::unique_ptr<juce::Drawable> brandMark;
    std::unique_ptr<juce::Drawable> safeBadge;
};

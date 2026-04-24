#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class RotaryKnobLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    enum class KnobRole
    {
        Primary,
        Support
    };

    void setAccentColour (juce::Colour newAccent) noexcept { accentColour = newAccent; }
    void setKnobRole (KnobRole newRole) noexcept { role = newRole; }

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                           juce::Slider& slider) override;

private:
    juce::Colour accentColour { juce::Colours::white };
    KnobRole role { KnobRole::Support };
};

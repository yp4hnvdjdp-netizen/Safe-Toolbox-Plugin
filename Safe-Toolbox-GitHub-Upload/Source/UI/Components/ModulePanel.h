#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class ModulePanel final : public juce::Component
{
public:
    enum class Style
    {
        Standard,
        SubtleFocused,
        Focused
    };

    ModulePanel (juce::String titleToUse, juce::Colour accentToUse, bool showPowerIndicator = false);

    void setTitle (juce::String newTitle);
    void setAccentColour (juce::Colour newAccent) noexcept { accent = newAccent; repaint(); }
    void setSubtitle (juce::String newSubtitle) { subtitle = std::move (newSubtitle); repaint(); }
    void setDetailText (juce::String newDetail) { detail = std::move (newDetail); repaint(); }
    void setStyle (Style newStyle) noexcept { style = newStyle; repaint(); }

    juce::Rectangle<int> getContentBounds() const noexcept;

    void paint (juce::Graphics& g) override;

private:
    juce::String title;
    juce::String subtitle;
    juce::String detail;
    juce::Colour accent;
    bool showPowerIndicator = false;
    Style style = Style::Standard;
    std::unique_ptr<juce::Drawable> textureOverlay;
};

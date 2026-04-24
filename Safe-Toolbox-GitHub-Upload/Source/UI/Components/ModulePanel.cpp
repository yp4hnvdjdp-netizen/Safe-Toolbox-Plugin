#include "ModulePanel.h"

#include "../Assets/AssetIds.h"
#include "../Assets/AssetLoader.h"
#include "../LookAndFeel/PluginColours.h"

ModulePanel::ModulePanel (juce::String titleToUse, juce::Colour accentToUse, bool showPowerIndicatorToUse)
    : title (std::move (titleToUse)),
      accent (accentToUse),
      showPowerIndicator (showPowerIndicatorToUse)
{
    setOpaque (true);
    setInterceptsMouseClicks (false, false);
    textureOverlay = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::panelTextureSvg);
}

void ModulePanel::setTitle (juce::String newTitle)
{
    title = std::move (newTitle);
    repaint();
}

juce::Rectangle<int> ModulePanel::getContentBounds() const noexcept
{
    auto inner = getLocalBounds().reduced (style == Style::Focused ? 14 : 12);
    inner.removeFromTop (style == Style::Focused ? 54 : style == Style::SubtleFocused ? 46 : 40);
    inner.removeFromBottom (10);
    if (style == Style::Focused)
        inner.reduce (3, 0);
    else if (style == Style::SubtleFocused)
        inner.reduce (2, 0);
    return inner;
}

void ModulePanel::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (1.0f);

    g.setColour (SafeToolboxTheme::panelFill());
    g.fillRoundedRectangle (bounds, 18.0f);

    if (textureOverlay != nullptr)
    {
        g.setOpacity (0.12f);
        textureOverlay->drawWithin (g, bounds.reduced (10.0f), juce::RectanglePlacement::stretchToFit, 1.0f);
        g.setOpacity (1.0f);
    }

    g.setColour (accent.withAlpha (0.14f));
    g.fillRoundedRectangle (bounds.reduced (9.0f, 9.0f).removeFromTop (6.0f), 3.0f);

    g.setColour (accent.withAlpha (0.94f));
    g.drawRoundedRectangle (bounds, 18.0f, 1.15f);

    auto header = getLocalBounds().removeFromTop (style == Style::Focused ? 50 : style == Style::SubtleFocused ? 40 : 34);
    g.setColour (accent.withAlpha (0.22f));
    g.fillRoundedRectangle (header.toFloat().reduced (8, 8), 10.0f);

    g.setColour (accent.withAlpha (0.98f));
    g.setFont (juce::FontOptions (style == Style::Focused ? 17.0f : style == Style::SubtleFocused ? 15.5f : 15.8f, juce::Font::bold));
    auto titleBounds = header.reduced (16, 0);
    titleBounds.removeFromBottom ((subtitle.isNotEmpty() ? (style == Style::SubtleFocused ? 12 : 14) : 0) + (detail.isNotEmpty() ? (style == Style::SubtleFocused ? 14 : 16) : 0));
    g.drawText (title, titleBounds, juce::Justification::centredLeft);

    if (subtitle.isNotEmpty())
    {
        auto subtitleBounds = header.reduced (16, 0).removeFromTop (style == Style::SubtleFocused ? 24 : 28);
        subtitleBounds.setY (subtitleBounds.getY() + (style == Style::SubtleFocused ? 16 : 19));
        g.setColour (juce::Colours::white.withAlpha (0.58f));
        g.setFont (juce::FontOptions (style == Style::SubtleFocused ? 9.2f : 10.0f, juce::Font::bold));
        g.drawText (subtitle.toUpperCase(), subtitleBounds, juce::Justification::centredLeft);
    }

    if (detail.isNotEmpty())
    {
        auto chip = header.removeFromRight (style == Style::Focused ? 118 : style == Style::SubtleFocused ? 90 : 84).reduced (0, style == Style::SubtleFocused ? 9 : 8);
        g.setColour (accent.withAlpha (style == Style::Focused ? 0.16f : 0.10f));
        g.fillRoundedRectangle (chip.toFloat(), 8.0f);
        g.setColour (accent.withAlpha (style == Style::Focused ? 0.84f : style == Style::SubtleFocused ? 0.60f : 0.70f));
        g.drawRoundedRectangle (chip.toFloat(), 8.0f, 1.0f);
        g.setColour (juce::Colours::white.withAlpha (style == Style::Focused ? 0.78f : style == Style::SubtleFocused ? 0.60f : 0.68f));
        g.setFont (juce::FontOptions (style == Style::Focused ? 9.4f : style == Style::SubtleFocused ? 8.4f : 8.8f, juce::Font::bold));
        g.drawText (detail.toUpperCase(), chip.reduced (8, 0), juce::Justification::centred);
    }

    auto dividerY = header.getBottom() - 1;
    g.setColour (accent.withAlpha (style == Style::Focused ? 0.54f : 0.32f));
    g.fillRect (header.getX() + 14, dividerY, header.getWidth() - 28, 1);

    if (style == Style::Focused)
    {
        g.setColour (accent.withAlpha (0.18f));
        g.fillRoundedRectangle (juce::Rectangle<float> ((float) header.getX() + 10.0f, (float) header.getBottom() - 5.0f, 5.0f, (float) getLocalBounds().getHeight() - header.getBottom() - 16.0f), 2.0f);

        auto rail = juce::Rectangle<float> ((float) getLocalBounds().getRight() - 12.0f, (float) header.getBottom() + 8.0f, 2.0f, (float) getLocalBounds().getBottom() - (float) header.getBottom() - 24.0f);
        g.setColour (accent.withAlpha (0.16f));
        g.fillRoundedRectangle (rail, 1.0f);
        g.setColour (accent.withAlpha (0.40f));
        g.fillRoundedRectangle (juce::Rectangle<float> (rail.getX() - 1.0f, rail.getY() + 22.0f, 4.0f, 22.0f), 1.5f);
        g.fillRoundedRectangle (juce::Rectangle<float> (rail.getX() - 1.0f, rail.getBottom() - 34.0f, 4.0f, 16.0f), 1.5f);
    }
    else if (style == Style::SubtleFocused)
    {
        auto rail = juce::Rectangle<float> ((float) getLocalBounds().getRight() - 11.0f, (float) header.getBottom() + 10.0f, 2.0f, (float) getLocalBounds().getBottom() - (float) header.getBottom() - 28.0f);
        g.setColour (accent.withAlpha (0.12f));
        g.fillRoundedRectangle (rail, 1.0f);
        g.setColour (accent.withAlpha (0.32f));
        g.fillRoundedRectangle (juce::Rectangle<float> (rail.getX() - 1.0f, rail.getY() + 20.0f, 4.0f, 14.0f), 1.5f);
    }

    auto headerUnderline = header.reduced (14, 0);
    headerUnderline.setY (headerUnderline.getBottom() - 2);
    headerUnderline.setHeight (1);
    g.setColour (accent.withAlpha (style == Style::Focused ? 0.48f : 0.30f));
    g.fillRect (headerUnderline);

    if (showPowerIndicator)
    {
        auto indicator = header.removeFromRight (24).reduced (6, 8);
        g.setColour (accent.withAlpha (0.90f));
        g.fillEllipse (indicator.toFloat());
        g.setColour (juce::Colours::white.withAlpha (0.14f));
        g.drawEllipse (indicator.toFloat(), 1.0f);
    }
}

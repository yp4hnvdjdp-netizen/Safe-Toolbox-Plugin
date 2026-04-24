#include "SafeToolboxLookAndFeel.h"

#include "../Assets/AssetIds.h"
#include "../Assets/AssetLoader.h"
#include "Theme.h"

namespace
{
    const juce::Drawable* iconForButtonText (const juce::String& text)
    {
        static const auto copyIcon = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::iconCopySvg);
        static const auto pasteIcon = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::iconPasteSvg);
        static const auto compareIcon = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::iconCompareSvg);
        static const auto panicIcon = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::iconPanicSvg);

        if (text.equalsIgnoreCase ("Copy"))
            return copyIcon.get();

        if (text.equalsIgnoreCase ("Paste"))
            return pasteIcon.get();

        if (text.equalsIgnoreCase ("Compare"))
            return compareIcon.get();

        if (text.containsIgnoreCase ("Panic"))
            return panicIcon.get();

        return nullptr;
    }
}

void SafeToolboxLookAndFeel::drawButtonBackground (juce::Graphics& g,
                                                   juce::Button& button,
                                                   const juce::Colour& backgroundColour,
                                                   bool shouldDrawButtonAsHighlighted,
                                                   bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused (button, backgroundColour);

    auto bounds = button.getLocalBounds().toFloat().reduced (0.5f);
    auto fill = SafeToolboxTheme::panelFill().withAlpha (0.96f);

    if (shouldDrawButtonAsDown)
        fill = accentColour.withAlpha (0.88f);
    else if (shouldDrawButtonAsHighlighted)
        fill = accentColour.withAlpha (0.52f);

    g.setColour (fill);
    g.fillRoundedRectangle (bounds, 7.0f);
    g.setColour (SafeToolboxTheme::panelOutline().withAlpha (0.95f));
    g.drawRoundedRectangle (bounds, 7.0f, 1.0f);
    g.setColour (juce::Colours::white.withAlpha (0.04f));
    g.drawRoundedRectangle (bounds.reduced (1.0f), 6.0f, 1.0f);
}

void SafeToolboxLookAndFeel::drawButtonText (juce::Graphics& g,
                                             juce::TextButton& button,
                                             bool shouldDrawButtonAsHighlighted,
                                             bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused (shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

    auto bounds = button.getLocalBounds().reduced (8, 0);

    if (const auto* icon = iconForButtonText (button.getButtonText()))
    {
        auto iconArea = bounds.removeFromLeft (16).withSizeKeepingCentre (14, 14);
        icon->drawWithin (g, iconArea.toFloat(), juce::RectanglePlacement::centred, 1.0f);
        bounds.removeFromLeft (4);
        g.setColour (SafeToolboxTheme::lightText());
        g.setFont (juce::FontOptions (button.getHeight() < 28 ? 10.4f : 11.0f, juce::Font::bold));
        g.drawFittedText (button.getButtonText(), bounds, juce::Justification::centredLeft, 1);
        return;
    }

    g.setColour (SafeToolboxTheme::lightText());
    g.setFont (juce::FontOptions (button.getHeight() < 28 ? 11.2f : 12.0f, juce::Font::bold));
    g.drawFittedText (button.getButtonText(), button.getLocalBounds().reduced (2, 0), juce::Justification::centred, 1);
}

void SafeToolboxLookAndFeel::drawComboBox (juce::Graphics& g, int width, int height, bool isButtonDown,
                                           int buttonX, int buttonY, int buttonW, int buttonH,
                                           juce::ComboBox& box)
{
    juce::ignoreUnused (isButtonDown, buttonX, buttonY, buttonW, buttonH);

    auto bounds = juce::Rectangle<float> (0.0f, 0.0f, (float) width, (float) height).reduced (0.5f);
    g.setColour (SafeToolboxTheme::panelFill());
    g.fillRoundedRectangle (bounds, 8.0f);
    g.setColour (SafeToolboxTheme::panelOutline());
    g.drawRoundedRectangle (bounds, 8.0f, 1.0f);

    auto textArea = bounds.toNearestInt().reduced (9, 0);
    auto arrowArea = textArea.removeFromRight (18);
    textArea.removeFromRight (5);

    g.setColour (SafeToolboxTheme::lightText().withAlpha (0.94f));
    g.setFont (juce::FontOptions (height < 30 ? 9.0f : 10.0f, juce::Font::bold));
    g.drawFittedText (box.getText(), textArea, juce::Justification::centredLeft, 1);

    juce::Path chevron;
    const auto cx = (float) arrowArea.getCentreX();
    const auto cy = (float) arrowArea.getCentreY();
    chevron.startNewSubPath (cx - 3.5f, cy - 1.0f);
    chevron.lineTo (cx, cy + 2.2f);
    chevron.lineTo (cx + 3.5f, cy - 1.0f);
    g.setColour (SafeToolboxTheme::mutedText().withAlpha (0.95f));
    g.strokePath (chevron, juce::PathStrokeType (1.6f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}

void SafeToolboxLookAndFeel::drawLabel (juce::Graphics& g, juce::Label& label)
{
    g.setColour (label.findColour (juce::Label::textColourId));
    g.setFont (juce::FontOptions (12.0f));
    g.drawFittedText (label.getText(), label.getLocalBounds(), label.getJustificationType(), 1);
}

void SafeToolboxLookAndFeel::drawPopupMenuBackground (juce::Graphics& g, int width, int height)
{
    g.setColour (SafeToolboxTheme::panelFill());
    g.fillRoundedRectangle (juce::Rectangle<float> (0.0f, 0.0f, (float) width, (float) height), 8.0f);
}

void SafeToolboxLookAndFeel::drawPopupMenuItem (juce::Graphics& g, const juce::Rectangle<int>& area,
                                                bool isSeparator, bool isActive, bool isHighlighted,
                                                bool isTicked, bool hasSubMenu, const juce::String& text,
                                                const juce::String& shortcutKeyText, const juce::Drawable* icon,
                                                const juce::Colour* textColour)
{
    juce::ignoreUnused (isSeparator, isTicked, hasSubMenu, shortcutKeyText, icon);

    if (isHighlighted)
    {
        g.setColour (accentColour.withAlpha (0.24f));
        g.fillRoundedRectangle (area.toFloat().reduced (2.0f), 6.0f);
    }

    g.setColour (isActive ? (textColour != nullptr ? *textColour : SafeToolboxTheme::lightText())
                          : SafeToolboxTheme::mutedText());
    g.setFont (juce::FontOptions (13.0f));
    g.drawText (text, area.reduced (8, 0), juce::Justification::centredLeft);
}

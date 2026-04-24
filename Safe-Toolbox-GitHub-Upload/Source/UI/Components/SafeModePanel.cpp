#include "SafeModePanel.h"

#include "../Assets/AssetIds.h"
#include "../Assets/AssetLoader.h"
#include "../LookAndFeel/PluginColours.h"

SafeModePanel::SafeModePanel (SafeToolboxAudioProcessor& p)
    : audioProcessor (p)
{
    setOpaque (true);
    badgeIcon = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::safeBadgeSvg);
    ledIcon = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::ledGreenSvg);
}

void SafeModePanel::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (1.0f);

    g.setColour (SafeToolboxTheme::safeModeFill());
    g.fillRoundedRectangle (bounds, 18.0f);

    g.setColour (SafeToolboxTheme::safeModeOutline().withAlpha (0.95f));
    g.drawRoundedRectangle (bounds, 18.0f, 1.1f);

    auto titleRow = getLocalBounds().reduced (16, 14).removeFromTop (28);
    g.setColour (SafeToolboxTheme::safeGreen());
    g.setFont (juce::FontOptions (17.0f, juce::Font::bold));
    if (badgeIcon != nullptr)
    {
        auto iconArea = titleRow.removeFromLeft (22);
        badgeIcon->drawWithin (g, iconArea.toFloat(), juce::RectanglePlacement::centred, 1.0f);
        titleRow.removeFromLeft (6);
    }
    g.drawText ("SAFE MODE", titleRow, juce::Justification::centredLeft);

    auto statusChip = getLocalBounds().reduced (22, 16).removeFromTop (26).removeFromRight (160);
    g.setColour (SafeToolboxTheme::safeGreen().withAlpha (0.12f));
    g.fillRoundedRectangle (statusChip.toFloat(), 12.0f);
    g.setColour (SafeToolboxTheme::safeGreen().withAlpha (0.60f));
    g.drawRoundedRectangle (statusChip.toFloat(), 12.0f, 1.0f);
    g.setColour (SafeToolboxTheme::safeGreen());
    g.setFont (juce::FontOptions (9.6f, juce::Font::bold));
    g.drawText ("PROTECTION ACTIVE", statusChip.reduced (10, 0), juce::Justification::centredRight);
    if (ledIcon != nullptr)
    {
        auto ledBounds = statusChip.removeFromLeft (20).reduced (1, 3);
        ledIcon->drawWithin (g, ledBounds.toFloat(), juce::RectanglePlacement::centred, 1.0f);
    }

    const auto limiterActive = audioProcessor.getMeterGainReductionDb() > 0.2f;
    const auto clipWarning = audioProcessor.getMeterClipWarning() > 0.5f;

    struct Row { const char* text; bool active; };
    const Row rows[] =
    {
        { "Ceiling guard active", true },
        { "Limiter active", limiterActive },
        { "DC blocker active", true },
        { "Invalid-sample guard active", true }
    };

    auto area = getLocalBounds().reduced (22, 48);
    area.removeFromTop (8);
    area.removeFromBottom (22);

    g.setColour (juce::Colours::white.withAlpha (0.88f));
    g.setFont (juce::FontOptions (11.8f));

    for (const auto& row : rows)
    {
        auto line = area.removeFromTop (20);
        if (row.active && ledIcon != nullptr)
        {
            auto ledBounds = juce::Rectangle<float> ((float) line.getX() - 12.0f, (float) line.getCentreY() - 8.0f, 16.0f, 16.0f);
            ledIcon->drawWithin (g, ledBounds, juce::RectanglePlacement::centred, 1.0f);
        }
        else
        {
            g.setColour (juce::Colours::white.withAlpha (0.34f));
            g.fillEllipse (line.getX() - 10, line.getCentreY() - 4, 8, 8);
        }
        g.setColour (juce::Colours::white.withAlpha (row.active ? 0.92f : 0.58f));
        g.drawText (row.text, line, juce::Justification::centredLeft);
    }

    auto footerRail = getLocalBounds().reduced (22, 18);
    footerRail.setY (footerRail.getBottom() - 2);
    footerRail.setHeight (1);
    g.setColour (SafeToolboxTheme::safeGreen().withAlpha (0.22f));
    g.fillRect (footerRail);

    if (clipWarning)
    {
        g.setColour (juce::Colours::red.withAlpha (0.88f));
        g.setFont (juce::FontOptions (11.0f, juce::Font::bold));
        g.drawText ("Clip warning engaged", getLocalBounds().reduced (22, 140).removeFromTop (18), juce::Justification::centredLeft);
    }
}

void SafeModePanel::resized()
{
}

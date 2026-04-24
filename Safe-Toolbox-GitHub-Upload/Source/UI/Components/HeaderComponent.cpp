#include "HeaderComponent.h"

#include "../Assets/AssetIds.h"
#include "../Assets/AssetLoader.h"
#include "../LookAndFeel/Theme.h"

HeaderComponent::HeaderComponent()
{
    setOpaque (true);
    brandMark = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::brandMarkSvg);
    safeBadge = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::safeBadgeSvg);
}

void HeaderComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (1.0f);

    g.setColour (SafeToolboxTheme::panelFill());
    g.fillRoundedRectangle (bounds, 18.0f);

    g.setColour (SafeToolboxTheme::panelOutline().withAlpha (0.95f));
    g.drawRoundedRectangle (bounds, 18.0f, 1.1f);

    auto content = getLocalBounds().reduced (18, 10);
    auto iconArea = content.removeFromLeft (54).reduced (2);
    auto titleArea = content.removeFromLeft (300);
    auto badgeArea = content.removeFromRight (290);

    if (brandMark != nullptr)
    {
        g.setOpacity (0.98f);
        brandMark->drawWithin (g, iconArea.reduced (2).toFloat(), juce::RectanglePlacement::centred, 1.0f);
        g.setOpacity (1.0f);
    }

    g.setColour (juce::Colours::white.withAlpha (0.96f));
    g.setFont (juce::FontOptions (22.5f, juce::Font::bold));
    g.drawText ("SAFE TOOLBOX", titleArea.removeFromTop (28).reduced (8, 0), juce::Justification::centredLeft);

    g.setColour (SafeToolboxTheme::mutedText());
    g.setFont (juce::FontOptions (11.0f));
    g.drawText ("SAFE AUDIO PLAYGROUND", titleArea.withTrimmedTop (24).reduced (8, 0), juce::Justification::centredLeft);

    auto badge = badgeArea.reduced (0, 10);
    g.setColour (SafeToolboxTheme::safeGreen().withAlpha (0.20f));
    g.fillRoundedRectangle (badge.toFloat().reduced (8, 8), 16.0f);
    g.setColour (SafeToolboxTheme::safeGreen());
    g.setFont (juce::FontOptions (10.8f, juce::Font::bold));
    g.drawText ("SAFE MODE   HEADPHONE PROTECTION", badge, juce::Justification::centredRight);

    if (safeBadge != nullptr)
    {
        auto badgeIcon = badge.removeFromLeft (28).withSizeKeepingCentre (18, 18);
        safeBadge->drawWithin (g, badgeIcon.toFloat(), juce::RectanglePlacement::centred, 1.0f);
    }

    g.fillEllipse (badge.getX() + 14, badge.getCentreY() - 4, 8, 8);
}

void HeaderComponent::resized()
{
}

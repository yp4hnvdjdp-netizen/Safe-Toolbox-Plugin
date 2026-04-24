#include "MeterComponent.h"

#include "../Assets/AssetIds.h"
#include "../Assets/AssetLoader.h"
#include "../LookAndFeel/PluginColours.h"

MeterComponent::MeterComponent (SafeToolboxAudioProcessor& p, MeterKind kindToUse)
    : audioProcessor (p), meterKind (kindToUse)
{
    setOpaque (true);
    setInterceptsMouseClicks (false, false);
    tickOverlay = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::meterTicksSvg);
    accentOverlay = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::meterAccentSvg);
}

void MeterComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (4.0f);
    const auto nowMs = juce::Time::getMillisecondCounterHiRes();
    const auto deltaSeconds = lastPaintMs > 0.0 ? (nowMs - lastPaintMs) * 0.001 : 0.0;
    lastPaintMs = nowMs;

    g.setColour (juce::Colour (0xaa0d1015));
    g.fillRoundedRectangle (bounds, 10.0f);
    g.setColour (juce::Colour (0xff303845).withAlpha (0.85f));
    g.drawRoundedRectangle (bounds, 10.0f, 1.0f);

    auto header = bounds.removeFromTop (20.0f);
    auto headerChip = header.reduced (2.0f, 2.0f);
    g.setColour (juce::Colours::white.withAlpha (0.05f));
    g.fillRoundedRectangle (headerChip, 7.0f);
    g.setColour (juce::Colours::white.withAlpha (0.16f));
    g.drawRoundedRectangle (headerChip, 7.0f, 1.0f);

    juce::String label = "IN";
    juce::Colour fill = juce::Colour (0xff59d37d);

    switch (meterKind)
    {
        case MeterKind::Input:
            label = "IN";
            fill = juce::Colour (0xff59d37d);
            break;
        case MeterKind::Output:
            label = "OUT";
            fill = juce::Colour (0xff66c9ff);
            break;
        case MeterKind::Reduction:
            label = "GR";
            fill = juce::Colour (0xffffb347);
            break;
    }

    g.setColour (fill.withAlpha (0.16f));
    g.fillRoundedRectangle (header.reduced (4.0f, 3.0f), 6.0f);
    g.setColour (fill.withAlpha (0.55f));
    g.drawRoundedRectangle (header.reduced (4.0f, 3.0f), 6.0f, 1.0f);
    g.setColour (juce::Colours::white.withAlpha (0.80f));
    g.setFont (juce::FontOptions (8.8f, juce::Font::bold));
    g.drawText (label, header.reduced (8, 0), juce::Justification::centredLeft);

    if (tickOverlay != nullptr)
    {
        g.setOpacity (0.72f);
        auto tickBounds = bounds.reduced (6.0f, 8.0f);
        tickBounds.removeFromTop (4.0f);
        tickOverlay->drawWithin (g, tickBounds, juce::RectanglePlacement::stretchToFit, 1.0f);
        g.setOpacity (1.0f);
    }

    float level = 0.0f;
    juce::Colour meterFill = juce::Colour (0xff59d37d);

    switch (meterKind)
    {
        case MeterKind::Input:
            level = juce::jlimit (0.0f, 1.0f, audioProcessor.getMeterInputPeak());
            meterFill = juce::Colour (0xff59d37d);
            break;
        case MeterKind::Output:
            level = juce::jlimit (0.0f, 1.0f, audioProcessor.getMeterOutputPeak());
            meterFill = juce::Colour (0xff66c9ff);
            break;
        case MeterKind::Reduction:
            level = juce::jlimit (0.0f, 1.0f, audioProcessor.getMeterGainReductionDb() / 12.0f);
            meterFill = juce::Colour (0xffffb347);
            break;
    }

    smoothedLevel = juce::jlimit (0.0f, 1.0f, smoothedLevel + (level - smoothedLevel) * juce::jlimit (0.12f, 0.38f, (float) (deltaSeconds * 8.0)));
    peakHoldLevel = juce::jmax (smoothedLevel, peakHoldLevel - (float) deltaSeconds * 0.18f);
    clipFlash = juce::jmax (clipFlash - (float) deltaSeconds * 2.2f, audioProcessor.getMeterClipWarning() > 0.5f ? 1.0f : 0.0f);

    auto meterBounds = bounds.withTrimmedTop (20.0f);
    const auto fillBounds = meterBounds.withTrimmedTop (meterBounds.getHeight() * (1.0f - smoothedLevel));
    g.setColour (meterFill.withAlpha (0.92f));
    g.fillRoundedRectangle (fillBounds, 9.0f);

    if (accentOverlay != nullptr)
    {
        g.setOpacity (meterKind == MeterKind::Reduction ? 0.98f : 0.78f);
        const auto accentBounds = juce::Rectangle<float> (meterBounds.getX() + 3.0f, fillBounds.getY() - 8.0f, meterBounds.getWidth() - 6.0f, 16.0f);
        accentOverlay->drawWithin (g, accentBounds, juce::RectanglePlacement::centred, 1.0f);
        g.setOpacity (1.0f);
    }

    g.setColour (juce::Colours::white.withAlpha (0.06f));
    g.drawLine (meterBounds.getX(), fillBounds.getY(), meterBounds.getRight(), fillBounds.getY(), 1.0f);

    const auto peakY = juce::jlimit ((float) meterBounds.getY(), (float) meterBounds.getBottom(), fillBounds.getY());
    g.setColour (meterKind == MeterKind::Reduction ? juce::Colour (0xffffb347) : juce::Colours::white.withAlpha (0.72f));
    g.fillRoundedRectangle (meterBounds.getX() + 4.0f, peakY - 2.0f, meterBounds.getWidth() - 8.0f, 4.0f, 2.0f);

    if (peakHoldLevel > 0.02f)
    {
        const auto holdY = meterBounds.getBottom() - (meterBounds.getHeight() * peakHoldLevel);
        g.setColour (meterKind == MeterKind::Output ? juce::Colour (0xff66c9ff) : juce::Colour (0xff59d37d));
        g.fillRoundedRectangle (meterBounds.getX() + 3.0f, holdY - 1.0f, meterBounds.getWidth() - 6.0f, 2.0f, 1.0f);
    }

    g.setColour (meterFill.withAlpha (0.22f));
    g.fillRoundedRectangle (meterBounds.getX() + 2.0f, meterBounds.getBottom() - 6.0f, meterBounds.getWidth() - 4.0f, 2.0f, 1.0f);

    if (clipFlash > 0.0f)
    {
        g.setColour (SafeToolboxTheme::safeGreen().withAlpha (juce::jlimit (0.0f, 1.0f, clipFlash)));
        g.fillEllipse (meterBounds.getRight() - 12.0f, meterBounds.getY() + 8.0f, 6.0f, 6.0f);
        if (clipFlash > 0.15f)
        {
            g.setColour (juce::Colours::red.withAlpha (juce::jlimit (0.0f, 1.0f, clipFlash)));
            g.fillEllipse (meterBounds.getRight() - 13.0f, meterBounds.getY() + 7.0f, 8.0f, 8.0f);
        }
    }
}

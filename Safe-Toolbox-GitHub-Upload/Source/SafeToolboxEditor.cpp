#include "SafeToolboxEditor.h"

#include "UI/Assets/AssetIds.h"
#include "Parameters/ParameterIDs.h"
#include "UI/LookAndFeel/Theme.h"

#ifndef SAFE_TOOLBOX_DEBUG_UI
#define SAFE_TOOLBOX_DEBUG_UI 0
#endif

namespace
{
    struct LayoutRects
    {
        juce::Rectangle<int> header;
        juce::Rectangle<int> presetBar;
        juce::Rectangle<int> modules;
        juce::Rectangle<int> safeMode;
        juce::Rectangle<int> meters;
    };

    float computeLayoutScale (juce::Rectangle<int> bounds)
    {
        const auto widthScale = (float) bounds.getWidth() / 1440.0f;
        const auto heightScale = (float) bounds.getHeight() / 860.0f;
        return juce::jlimit (0.90f, 1.0f, juce::jmin (widthScale, heightScale));
    }

    int scaled (float scale, int value)
    {
        return juce::roundToInt (scale * (float) value);
    }

    LayoutRects makeLayout (juce::Rectangle<int> bounds)
    {
        const auto scale = computeLayoutScale (bounds);
        auto area = bounds.reduced (scaled (scale, 16));

        LayoutRects r;
        r.header = area.removeFromTop (scaled (scale, 78));
        area.removeFromTop (scaled (scale, 8));
        r.presetBar = area.removeFromTop (scaled (scale, 40));
        area.removeFromTop (scaled (scale, 10));
        r.modules = area.removeFromTop (scaled (scale, 402));
        area.removeFromTop (scaled (scale, 10));

        auto bottom = area;
        r.meters = bottom.removeFromRight (juce::jlimit (scaled (scale, 236), scaled (scale, 300), bottom.getWidth() / 3));
        r.safeMode = bottom;
        return r;
    }

    void appendDebugLine (const juce::String& line)
    {
       #if SAFE_TOOLBOX_DEBUG_UI
        auto debugLogFile = []()
        {
            return juce::File ("/Users/josephshively/Documents/New project/debug/safe-toolbox-ui-debug.log");
        };

        auto file = debugLogFile();
        file.getParentDirectory().createDirectory();
        file.appendText (juce::Time::getCurrentTime().toString (true, true, true, true) + " | " + line + "\n", false, false, nullptr);
        DBG (line);
       #else
        juce::ignoreUnused (line);
       #endif
    }

    void drawOuterFrame (juce::Graphics& g, juce::Rectangle<int> bounds)
    {
        g.setColour (SafeToolboxTheme::shellFill().withAlpha (0.6f));
        g.fillRoundedRectangle (bounds.toFloat().reduced (12.0f), 22.0f);
        g.setColour (SafeToolboxTheme::shellOutline().withAlpha (0.9f));
        g.drawRoundedRectangle (bounds.toFloat().reduced (12.0f), 22.0f, 1.0f);
    }

}

SafeToolboxAudioProcessorEditor::SafeToolboxAudioProcessorEditor (SafeToolboxAudioProcessor& p)
    : AudioProcessorEditor (&p),
      audioProcessor (p),
      presetBar (p),
      inputPanel ("INPUT", SafeToolboxTheme::neutralAccent(), true),
      drivePanel ("DRIVE", SafeToolboxTheme::driveAccent()),
      filterPanel ("FILTER", SafeToolboxTheme::filterAccent()),
      delayPanel ("DELAY", SafeToolboxTheme::delayAccent()),
      reverbPanel ("REVERB", SafeToolboxTheme::reverbAccent()),
      wobblePanel ("WOBBLE", SafeToolboxTheme::wobbleAccent()),
      widthPanel ("WIDTH", SafeToolboxTheme::widthAccent()),
      outputPanel ("OUTPUT", SafeToolboxTheme::neutralAccent(), true),
      safeModePanel (p),
      inputMeter (p, MeterComponent::MeterKind::Input),
      outputMeter (p, MeterComponent::MeterKind::Output),
      reductionMeter (p, MeterComponent::MeterKind::Reduction)
{
    setOpaque (true);
    setResizable (true, true);
    setResizeLimits (1280, 780, 2200, 1400);
    setSize (1440, 860);
    setLookAndFeel (&uiLaf);
    backgroundArtwork = SafeToolboxAssets::loadImage (SafeToolboxAssets::Ids::backgroundArtworkPng);
    backdropTexture = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::backdropTextureSvg);

    headerComponent.setName ("Header");
    presetBar.setName ("Preset Bar");
    inputPanel.setName ("Input Panel");
    drivePanel.setName ("Drive Panel");
    filterPanel.setName ("Filter Panel");
    delayPanel.setName ("Delay Panel");
    reverbPanel.setName ("Reverb Panel");
    wobblePanel.setName ("Wobble Panel");
    widthPanel.setName ("Width Panel");
    outputPanel.setName ("Output Panel");
    safeModePanel.setName ("Safe Mode");
    inputMeter.setName ("Input Meter");
    outputMeter.setName ("Output Meter");
    reductionMeter.setName ("Gain Reduction Meter");

    neutralLaf.setAccentColour (juce::Colour (0xff6a717d));
    driveLaf.setAccentColour (juce::Colour (0xffff7a59));
    driveLaf.setKnobRole (RotaryKnobLookAndFeel::KnobRole::Primary);
    driveSupportLaf.setAccentColour (juce::Colour (0xffff7a59));
    driveSupportLaf.setKnobRole (RotaryKnobLookAndFeel::KnobRole::Support);
    filterLaf.setAccentColour (juce::Colour (0xffffd34f));
    filterLaf.setKnobRole (RotaryKnobLookAndFeel::KnobRole::Primary);
    filterSupportLaf.setAccentColour (juce::Colour (0xffffd34f));
    filterSupportLaf.setKnobRole (RotaryKnobLookAndFeel::KnobRole::Support);
    delayLaf.setAccentColour (juce::Colour (0xff78e7ff));
    delayLaf.setKnobRole (RotaryKnobLookAndFeel::KnobRole::Primary);
    delaySupportLaf.setAccentColour (juce::Colour (0xff78e7ff));
    delaySupportLaf.setKnobRole (RotaryKnobLookAndFeel::KnobRole::Support);
    reverbLaf.setAccentColour (juce::Colour (0xffcc7bff));
    reverbLaf.setKnobRole (RotaryKnobLookAndFeel::KnobRole::Primary);
    reverbSupportLaf.setAccentColour (juce::Colour (0xffcc7bff));
    reverbSupportLaf.setKnobRole (RotaryKnobLookAndFeel::KnobRole::Support);
    wobbleLaf.setAccentColour (juce::Colour (0xff7eb8ff));
    widthLaf.setAccentColour (juce::Colour (0xfff072c2));
    outputLaf.setAccentColour (juce::Colour (0xfff28c6b));

    drivePanel.setSubtitle ("HARMONICS");
    drivePanel.setStyle (ModulePanel::Style::Focused);
    drivePanel.setDetailText ("SATURATE");
    filterPanel.setSubtitle ("TONE SHAPING");
    filterPanel.setStyle (ModulePanel::Style::Focused);
    filterPanel.setDetailText ("CUT / RES");
    delayPanel.setSubtitle ("TIME / SPACE");
    delayPanel.setStyle (ModulePanel::Style::Focused);
    delayPanel.setDetailText ("ECHO");
    reverbPanel.setSubtitle ("AMBIENCE");
    reverbPanel.setStyle (ModulePanel::Style::Focused);
    reverbPanel.setDetailText ("SPACE");
    wobblePanel.setSubtitle ("MOTION");
    wobblePanel.setStyle (ModulePanel::Style::SubtleFocused);
    wobblePanel.setDetailText ("DRIFT");
    widthPanel.setSubtitle ("STEREO FIELD");
    widthPanel.setStyle (ModulePanel::Style::SubtleFocused);
    widthPanel.setDetailText ("IMAGE");
    outputPanel.setSubtitle ("FINAL STAGE");
    inputPanel.setSubtitle ("SOURCE");
    inputPanel.setDetailText ("TRIM");
    outputPanel.setDetailText ("LIMIT");

    widthMonoBelowButton.setButtonText ("MONO");
    panicMuteButton.setButtonText ("PANIC");

    addAndMakeVisible (headerComponent);
    addAndMakeVisible (presetBar);
    addAndMakeVisible (inputPanel);
    addAndMakeVisible (drivePanel);
    addAndMakeVisible (filterPanel);
    addAndMakeVisible (delayPanel);
    addAndMakeVisible (reverbPanel);
    addAndMakeVisible (wobblePanel);
    addAndMakeVisible (widthPanel);
    addAndMakeVisible (outputPanel);
    addAndMakeVisible (safeModePanel);
    addAndMakeVisible (inputMeter);
    addAndMakeVisible (outputMeter);
    addAndMakeVisible (reductionMeter);

    configureSlider (inputGainSlider, "Input gain", SafeToolboxTheme::neutralAccent(), neutralLaf);

    configureSlider (driveAmountSlider, "Drive amount", SafeToolboxTheme::driveAccent(), driveLaf);
    configureSlider (driveToneSlider, "Drive tone", SafeToolboxTheme::driveAccent(), driveSupportLaf);
    configureSlider (driveMixSlider, "Drive mix", SafeToolboxTheme::driveAccent(), driveSupportLaf);
    configureSlider (driveOutputSlider, "Drive output", SafeToolboxTheme::driveAccent(), driveSupportLaf);
    configureButton (driveBypassButton, "Drive bypass", juce::Colour (0xff22262e), juce::Colour (0xff5e7c92));

    configureComboBox (filterTypeBox, "Filter type", SafeToolboxParamIDs::filterTypeChoices());
    configureSlider (filterCutoffSlider, "Filter cutoff", SafeToolboxTheme::filterAccent(), filterLaf);
    configureSlider (filterResonanceSlider, "Filter resonance", SafeToolboxTheme::filterAccent(), filterSupportLaf);
    configureSlider (filterDriveSlider, "Filter drive", SafeToolboxTheme::filterAccent(), filterSupportLaf);
    configureComboBox (filterSlopeBox, "Filter slope", SafeToolboxParamIDs::filterSlopeChoices());
    configureButton (filterBypassButton, "Filter bypass", juce::Colour (0xff252720), juce::Colour (0xff7d8d4c));

    configureComboBox (delayModeBox, "Delay mode", SafeToolboxParamIDs::delayModeChoices());
    configureComboBox (delaySyncBox, "Delay sync", SafeToolboxParamIDs::delaySyncChoices());
    configureSlider (delayTimeSlider, "Delay time", SafeToolboxTheme::delayAccent(), delayLaf);
    configureSlider (delayFeedbackSlider, "Delay feedback", SafeToolboxTheme::delayAccent(), delaySupportLaf);
    configureSlider (delayDampingSlider, "Delay damping", SafeToolboxTheme::delayAccent(), delaySupportLaf);
    configureSlider (delayMixSlider, "Delay mix", SafeToolboxTheme::delayAccent(), delaySupportLaf);
    configureButton (delayBypassButton, "Delay bypass", juce::Colour (0xff18262a), juce::Colour (0xff3c8fa6));

    configureComboBox (reverbModeBox, "Reverb mode", SafeToolboxParamIDs::reverbModeChoices());
    configureSlider (reverbSizeSlider, "Reverb size", SafeToolboxTheme::reverbAccent(), reverbLaf);
    configureSlider (reverbDecaySlider, "Reverb decay", SafeToolboxTheme::reverbAccent(), reverbSupportLaf);
    configureSlider (reverbDampingSlider, "Reverb damping", SafeToolboxTheme::reverbAccent(), reverbSupportLaf);
    configureSlider (reverbWidthSlider, "Reverb width", SafeToolboxTheme::reverbAccent(), reverbSupportLaf);
    configureSlider (reverbMixSlider, "Reverb mix", SafeToolboxTheme::reverbAccent(), reverbSupportLaf);
    configureButton (reverbBypassButton, "Reverb bypass", juce::Colour (0xff261d2b), juce::Colour (0xff8640a8));

    configureComboBox (wobbleModeBox, "Wobble mode", SafeToolboxParamIDs::wobbleModeChoices());
    configureSlider (wobbleRateSlider, "Wobble rate", SafeToolboxTheme::wobbleAccent(), wobbleLaf);
    configureSlider (wobbleDepthSlider, "Wobble depth", SafeToolboxTheme::wobbleAccent(), wobbleLaf);
    configureSlider (wobbleShapeSlider, "Wobble shape", SafeToolboxTheme::wobbleAccent(), wobbleLaf);
    configureSlider (wobblePhaseSlider, "Wobble stereo phase", SafeToolboxTheme::wobbleAccent(), wobbleLaf);
    configureSlider (wobbleMixSlider, "Wobble mix", SafeToolboxTheme::wobbleAccent(), wobbleLaf);
    configureButton (wobbleBypassButton, "Wobble bypass", juce::Colour (0xff1f2a36), juce::Colour (0xff487ec4));

    configureSlider (widthSlider, "Stereo width", SafeToolboxTheme::widthAccent(), widthLaf);
    configureSlider (widthLowCutSlider, "Mono bass frequency", SafeToolboxTheme::widthAccent(), widthLaf);
    configureSlider (widthBalanceSlider, "Mid / side balance", SafeToolboxTheme::widthAccent(), widthLaf);
    configureSlider (widthHighCutSlider, "Width high cut", SafeToolboxTheme::widthAccent(), widthLaf);
    configureButton (widthMonoBelowButton, "Mono below toggle", juce::Colour (0xff1b2621), juce::Colour (0xff2a8f66));
    configureButton (widthBypassButton, "Width bypass", juce::Colour (0xff231b23), juce::Colour (0xff8d3c72));

    configureSlider (outputGainSlider, "Output gain", SafeToolboxTheme::neutralAccent(), outputLaf);
    configureSlider (safetyCeilingSlider, "Safety ceiling", SafeToolboxTheme::neutralAccent(), outputLaf);
    configureSlider (limiterThresholdSlider, "Limiter strength", SafeToolboxTheme::neutralAccent(), outputLaf);
    configureSlider (limiterReleaseSlider, "Limiter release", SafeToolboxTheme::neutralAccent(), outputLaf);
    configureSlider (softClipSlider, "Soft clip", SafeToolboxTheme::neutralAccent(), outputLaf);
    configureButton (panicMuteButton, "Panic mute", juce::Colour (0xff2a1b1b), juce::Colour (0xff8f2b2b));

    inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::inputGain, inputGainSlider);

    driveAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::driveAmount, driveAmountSlider);
    driveToneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::driveTone, driveToneSlider);
    driveMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::driveMix, driveMixSlider);
    driveOutputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::driveOutput, driveOutputSlider);
    driveBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::driveBypass, driveBypassButton);

    filterTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::filterType, filterTypeBox);
    filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::filterCutoff, filterCutoffSlider);
    filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::filterResonance, filterResonanceSlider);
    filterDriveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::filterDrive, filterDriveSlider);
    filterSlopeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::filterSlope, filterSlopeBox);
    filterBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::filterBypass, filterBypassButton);

    delayModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::delayMode, delayModeBox);
    delaySyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::delaySync, delaySyncBox);
    delayTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::delayTime, delayTimeSlider);
    delayFeedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::delayFeedback, delayFeedbackSlider);
    delayDampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::delayDamping, delayDampingSlider);
    delayMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::delayMix, delayMixSlider);
    delayBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::delayBypass, delayBypassButton);

    reverbModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::reverbMode, reverbModeBox);
    reverbSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::reverbSize, reverbSizeSlider);
    reverbDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::reverbDecay, reverbDecaySlider);
    reverbDampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::reverbDamping, reverbDampingSlider);
    reverbWidthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::reverbWidth, reverbWidthSlider);
    reverbMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::reverbMix, reverbMixSlider);
    reverbBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::reverbBypass, reverbBypassButton);

    wobbleModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::wobbleMode, wobbleModeBox);
    wobbleRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::wobbleRate, wobbleRateSlider);
    wobbleDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::wobbleDepth, wobbleDepthSlider);
    wobbleShapeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::wobbleShape, wobbleShapeSlider);
    wobblePhaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::wobbleStereoPhase, wobblePhaseSlider);
    wobbleMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::wobbleMix, wobbleMixSlider);
    wobbleBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::wobbleBypass, wobbleBypassButton);

    widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::width, widthSlider);
    widthLowCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::widthLowCut, widthLowCutSlider);
    widthBalanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::balance, widthBalanceSlider);
    widthHighCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::widthHighCut, widthHighCutSlider);
    widthMonoBelowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::widthMonoBelow, widthMonoBelowButton);
    widthBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::widthBypass, widthBypassButton);

    outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::outputGain, outputGainSlider);
    safetyCeilingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::safetyCeiling, safetyCeilingSlider);
    limiterThresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::limiterThreshold, limiterThresholdSlider);
    limiterReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::limiterRelease, limiterReleaseSlider);
    softClipAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::softClip, softClipSlider);
    panicMuteAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, SafeToolboxParamIDs::panicMute, panicMuteButton);

    appendDebugLine ("ctor size=" + juce::String (getWidth()) + "x" + juce::String (getHeight())
                     + " scale=" + juce::String (juce::Desktop::getInstance().getGlobalScaleFactor(), 2));
    logDebugLayout ("ctor");
    resized();
    startTimerHz (30);
}

SafeToolboxAudioProcessorEditor::~SafeToolboxAudioProcessorEditor()
{
    setLookAndFeel (nullptr);

    inputGainSlider.setLookAndFeel (nullptr);
    driveAmountSlider.setLookAndFeel (nullptr);
    driveToneSlider.setLookAndFeel (nullptr);
    driveMixSlider.setLookAndFeel (nullptr);
    driveOutputSlider.setLookAndFeel (nullptr);
    filterCutoffSlider.setLookAndFeel (nullptr);
    filterResonanceSlider.setLookAndFeel (nullptr);
    filterDriveSlider.setLookAndFeel (nullptr);
    delayTimeSlider.setLookAndFeel (nullptr);
    delayFeedbackSlider.setLookAndFeel (nullptr);
    delayDampingSlider.setLookAndFeel (nullptr);
    delayMixSlider.setLookAndFeel (nullptr);
    reverbSizeSlider.setLookAndFeel (nullptr);
    reverbDecaySlider.setLookAndFeel (nullptr);
    reverbDampingSlider.setLookAndFeel (nullptr);
    reverbWidthSlider.setLookAndFeel (nullptr);
    reverbMixSlider.setLookAndFeel (nullptr);
    wobbleRateSlider.setLookAndFeel (nullptr);
    wobbleDepthSlider.setLookAndFeel (nullptr);
    wobbleShapeSlider.setLookAndFeel (nullptr);
    wobblePhaseSlider.setLookAndFeel (nullptr);
    wobbleMixSlider.setLookAndFeel (nullptr);
    widthSlider.setLookAndFeel (nullptr);
    widthLowCutSlider.setLookAndFeel (nullptr);
    widthBalanceSlider.setLookAndFeel (nullptr);
    widthHighCutSlider.setLookAndFeel (nullptr);
    outputGainSlider.setLookAndFeel (nullptr);
    safetyCeilingSlider.setLookAndFeel (nullptr);
    limiterThresholdSlider.setLookAndFeel (nullptr);
    limiterReleaseSlider.setLookAndFeel (nullptr);
    softClipSlider.setLookAndFeel (nullptr);
}

void SafeToolboxAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::ColourGradient bg (juce::Colour (0xff11151b), 0.0f, 0.0f,
                             juce::Colour (0xff07080b), 0.0f, (float) getHeight(), false);
    g.setGradientFill (bg);
    g.fillAll();

    if (backgroundArtwork.isValid())
    {
        g.setOpacity (0.22f);
        g.drawImage (backgroundArtwork, getLocalBounds().toFloat(), juce::RectanglePlacement::centred | juce::RectanglePlacement::fillDestination);
        g.setOpacity (1.0f);
    }

    if (backdropTexture != nullptr)
    {
        g.setOpacity (0.16f);
        backdropTexture->drawWithin (g, getLocalBounds().toFloat().reduced (20.0f), juce::RectanglePlacement::stretchToFit, 1.0f);
        g.setOpacity (1.0f);
    }

    drawOuterFrame (g, getLocalBounds());
}

void SafeToolboxAudioProcessorEditor::paintOverChildren (juce::Graphics& g)
{
#if SAFE_TOOLBOX_DEBUG_UI
    auto drawOutline = [&g] (const juce::Component& c, juce::Colour colour, juce::Colour textColour)
    {
        auto b = c.getBounds().toFloat();
        g.setColour (colour);
        g.drawRect (b, 1.0f);

        if (c.getName().isNotEmpty())
        {
            auto label = b.withHeight (16.0f).reduced (2.0f);
            g.setColour (textColour);
            g.setFont (juce::FontOptions (10.0f, juce::Font::bold));
            g.drawText (c.getName(), label.toNearestInt(), juce::Justification::centredLeft, false);
        }
    };

    g.setOpacity (0.8f);
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        auto* child = getChildComponent (i);
        if (child == nullptr || ! child->isVisible())
            continue;

        if (dynamic_cast<ModulePanel*> (child) != nullptr)
            drawOutline (*child, juce::Colour (0xff57f2ff), juce::Colour (0xffbff9ff));
        else if (dynamic_cast<juce::Slider*> (child) != nullptr)
            drawOutline (*child, juce::Colour (0xffff7a59), juce::Colour (0xffffd0bf));
        else if (dynamic_cast<juce::ComboBox*> (child) != nullptr)
            drawOutline (*child, juce::Colour (0xffd1ff57), juce::Colour (0xffeefcb2));
        else if (dynamic_cast<juce::TextButton*> (child) != nullptr)
            drawOutline (*child, juce::Colour (0xffa76bff), juce::Colour (0xffe2ccff));
        else if (dynamic_cast<MeterComponent*> (child) != nullptr)
            drawOutline (*child, juce::Colour (0xff44d17d), juce::Colour (0xffd4ffe3));
        else if (dynamic_cast<SafeModePanel*> (child) != nullptr)
            drawOutline (*child, juce::Colour (0xffffd34f), juce::Colour (0xffffefac));
        else
            drawOutline (*child, juce::Colour (0xff7f8796), juce::Colour (0xffd5d8de));
    }

    auto drawPanelBounds = [&g] (const ModulePanel& panel, juce::Colour colour)
    {
        auto panelBounds = panel.getBounds().toFloat();
        g.setColour (colour);
        g.drawRect (panelBounds, 1.0f);

        auto content = panel.getContentBounds().toFloat().translated ((float) panel.getX(), (float) panel.getY());
        g.setColour (colour.withAlpha (0.40f));
        g.drawRect (content, 1.0f);
    };

    drawPanelBounds (inputPanel, juce::Colour (0xff57f2ff));
    drawPanelBounds (drivePanel, juce::Colour (0xffff7a59));
    drawPanelBounds (filterPanel, juce::Colour (0xffffd34f));
    drawPanelBounds (delayPanel, juce::Colour (0xff78e7ff));
    drawPanelBounds (reverbPanel, juce::Colour (0xffcc7bff));
    drawPanelBounds (wobblePanel, juce::Colour (0xff7eb8ff));
    drawPanelBounds (widthPanel, juce::Colour (0xfff072c2));
    drawPanelBounds (outputPanel, juce::Colour (0xfff28c6b));
#else
    juce::ignoreUnused (g);
#endif
}

void SafeToolboxAudioProcessorEditor::resized()
{
    auto localBounds = getLocalBounds();
    auto rects = makeLayout (localBounds);
    const auto scale = computeLayoutScale (localBounds);

    appendDebugLine ("resized size=" + juce::String (getWidth()) + "x" + juce::String (getHeight())
                     + " scale=" + juce::String (juce::Desktop::getInstance().getGlobalScaleFactor(), 2)
                     + " layoutScale=" + juce::String (scale, 2)
                     + " header=" + rects.header.toString()
                     + " preset=" + rects.presetBar.toString()
                     + " modules=" + rects.modules.toString()
                     + " safe=" + rects.safeMode.toString()
                     + " meters=" + rects.meters.toString());

    headerComponent.setBounds (rects.header);
    presetBar.setBounds (rects.presetBar);

    const int gap = scaled (scale, 8);
    const int panelWidth = (rects.modules.getWidth() - gap * 7) / 8;
    const int x0 = rects.modules.getX();

    auto placeLarge = [scale] (const juce::Rectangle<int>& content, int size)
    {
        const auto knob = scaled (scale, size);
        return juce::Rectangle<int> (content.getCentreX() - knob / 2, content.getY() + scaled (scale, 6), knob, knob + scaled (scale, 12));
    };

    auto placeSmall = [scale] (const juce::Rectangle<int>& content, int index, int width, int height, int offsetY)
    {
        const int w = scaled (scale, width);
        const int h = scaled (scale, height);
        const int x = index == 0 ? content.getX() + scaled (scale, 6) : content.getRight() - w - scaled (scale, 6);
        return juce::Rectangle<int> (x, content.getY() + scaled (scale, offsetY), w, h);
    };

    auto placeWide = [scale] (const juce::Rectangle<int>& content, int offsetY, int height)
    {
        return juce::Rectangle<int> (content.getX() + scaled (scale, 4), content.getY() + scaled (scale, offsetY), content.getWidth() - scaled (scale, 8), scaled (scale, height));
    };

    auto toEditorBounds = [] (const juce::Component& owner, juce::Rectangle<int> bounds)
    {
        return bounds.translated (owner.getX(), owner.getY());
    };

    // Input
    inputPanel.setBounds (x0, rects.modules.getY(), panelWidth, rects.modules.getHeight());
    {
        auto c = inputPanel.getContentBounds();
        inputGainSlider.setBounds (toEditorBounds (inputPanel, placeLarge (c, 80)));
        appendDebugLine ("inputPanel=" + inputPanel.getBounds().toString() + " content=" + c.toString()
                         + " inputGain=" + inputGainSlider.getBounds().toString());
    }

    // Drive
    drivePanel.setBounds (x0 + (panelWidth + gap), rects.modules.getY(), panelWidth, rects.modules.getHeight());
    {
        auto c = drivePanel.getContentBounds();
        driveAmountSlider.setBounds (toEditorBounds (drivePanel, placeLarge (c, 90)));
        driveToneSlider.setBounds (toEditorBounds (drivePanel, placeSmall (c, 0, 54, 54, 114)));
        driveMixSlider.setBounds (toEditorBounds (drivePanel, placeSmall (c, 1, 54, 54, 114)));
        driveOutputSlider.setBounds (toEditorBounds (drivePanel, placeSmall (c, 0, 54, 54, 186)));
        driveBypassButton.setBounds (toEditorBounds (drivePanel, placeWide (c, 216, 28)));
        appendDebugLine ("drivePanel=" + drivePanel.getBounds().toString() + " content=" + c.toString()
                         + " amount=" + driveAmountSlider.getBounds().toString()
                         + " tone=" + driveToneSlider.getBounds().toString()
                         + " mix=" + driveMixSlider.getBounds().toString()
                         + " output=" + driveOutputSlider.getBounds().toString()
                         + " bypass=" + driveBypassButton.getBounds().toString());
    }

    // Filter
    filterPanel.setBounds (x0 + 2 * (panelWidth + gap), rects.modules.getY(), panelWidth, rects.modules.getHeight());
    {
        auto c = filterPanel.getContentBounds();
        filterCutoffSlider.setBounds (toEditorBounds (filterPanel, placeLarge (c, 90)));
        filterResonanceSlider.setBounds (toEditorBounds (filterPanel, placeSmall (c, 0, 54, 54, 114)));
        filterDriveSlider.setBounds (toEditorBounds (filterPanel, placeSmall (c, 1, 54, 54, 114)));
        filterTypeBox.setBounds (toEditorBounds (filterPanel, placeWide (c, 196, 28)));
        filterSlopeBox.setBounds (toEditorBounds (filterPanel, placeWide (c, 228, 26)));
        filterBypassButton.setBounds (toEditorBounds (filterPanel, placeWide (c, 260, 26)));
        appendDebugLine ("filterPanel=" + filterPanel.getBounds().toString() + " content=" + c.toString()
                         + " cutoff=" + filterCutoffSlider.getBounds().toString()
                         + " resonance=" + filterResonanceSlider.getBounds().toString()
                         + " drive=" + filterDriveSlider.getBounds().toString()
                         + " type=" + filterTypeBox.getBounds().toString()
                         + " slope=" + filterSlopeBox.getBounds().toString()
                         + " bypass=" + filterBypassButton.getBounds().toString());
    }

    // Delay
    delayPanel.setBounds (x0 + 3 * (panelWidth + gap), rects.modules.getY(), panelWidth, rects.modules.getHeight());
    {
        auto c = delayPanel.getContentBounds();
        delayTimeSlider.setBounds (toEditorBounds (delayPanel, placeLarge (c, 84)));
        delayFeedbackSlider.setBounds (toEditorBounds (delayPanel, placeSmall (c, 0, 54, 54, 106)));
        delayDampingSlider.setBounds (toEditorBounds (delayPanel, placeSmall (c, 1, 54, 54, 106)));
        delayMixSlider.setBounds (toEditorBounds (delayPanel, placeSmall (c, 0, 54, 54, 188)));
        delaySyncBox.setBounds (toEditorBounds (delayPanel, placeWide (c, 214, 26)));
        delayModeBox.setBounds (toEditorBounds (delayPanel, placeWide (c, 246, 28)));
        delayBypassButton.setBounds (toEditorBounds (delayPanel, placeWide (c, 280, 26)));
        appendDebugLine ("delayPanel=" + delayPanel.getBounds().toString() + " content=" + c.toString()
                         + " time=" + delayTimeSlider.getBounds().toString()
                         + " feedback=" + delayFeedbackSlider.getBounds().toString()
                         + " damping=" + delayDampingSlider.getBounds().toString()
                         + " mix=" + delayMixSlider.getBounds().toString()
                         + " sync=" + delaySyncBox.getBounds().toString()
                         + " mode=" + delayModeBox.getBounds().toString()
                         + " bypass=" + delayBypassButton.getBounds().toString());
    }

    // Reverb
    reverbPanel.setBounds (x0 + 4 * (panelWidth + gap), rects.modules.getY(), panelWidth, rects.modules.getHeight());
    {
        auto c = reverbPanel.getContentBounds();
        reverbSizeSlider.setBounds (toEditorBounds (reverbPanel, placeLarge (c, 84)));
        reverbDecaySlider.setBounds (toEditorBounds (reverbPanel, placeSmall (c, 0, 54, 54, 106)));
        reverbDampingSlider.setBounds (toEditorBounds (reverbPanel, placeSmall (c, 1, 54, 54, 106)));
        reverbWidthSlider.setBounds (toEditorBounds (reverbPanel, placeSmall (c, 0, 54, 54, 188)));
        reverbMixSlider.setBounds (toEditorBounds (reverbPanel, placeSmall (c, 1, 54, 54, 188)));
        reverbModeBox.setBounds (toEditorBounds (reverbPanel, placeWide (c, 248, 28)));
        reverbBypassButton.setBounds (toEditorBounds (reverbPanel, placeWide (c, 280, 26)));
        appendDebugLine ("reverbPanel=" + reverbPanel.getBounds().toString() + " content=" + c.toString()
                         + " size=" + reverbSizeSlider.getBounds().toString()
                         + " decay=" + reverbDecaySlider.getBounds().toString()
                         + " damping=" + reverbDampingSlider.getBounds().toString()
                         + " width=" + reverbWidthSlider.getBounds().toString()
                         + " mix=" + reverbMixSlider.getBounds().toString()
                         + " mode=" + reverbModeBox.getBounds().toString()
                         + " bypass=" + reverbBypassButton.getBounds().toString());
    }

    // Wobble
    wobblePanel.setBounds (x0 + 5 * (panelWidth + gap), rects.modules.getY(), panelWidth, rects.modules.getHeight());
    {
        auto c = wobblePanel.getContentBounds();
        wobbleRateSlider.setBounds (toEditorBounds (wobblePanel, placeLarge (c, 76)));
        wobbleDepthSlider.setBounds (toEditorBounds (wobblePanel, placeSmall (c, 0, 54, 54, 96)));
        wobbleShapeSlider.setBounds (toEditorBounds (wobblePanel, placeSmall (c, 1, 54, 54, 96)));
        wobblePhaseSlider.setBounds (toEditorBounds (wobblePanel, placeSmall (c, 0, 54, 54, 170)));
        wobbleMixSlider.setBounds (toEditorBounds (wobblePanel, placeSmall (c, 1, 54, 54, 170)));
        wobbleModeBox.setBounds (toEditorBounds (wobblePanel, placeWide (c, 232, 28)));
        wobbleBypassButton.setBounds (toEditorBounds (wobblePanel, placeWide (c, 264, 26)));
        appendDebugLine ("wobblePanel=" + wobblePanel.getBounds().toString() + " content=" + c.toString()
                         + " rate=" + wobbleRateSlider.getBounds().toString()
                         + " depth=" + wobbleDepthSlider.getBounds().toString()
                         + " shape=" + wobbleShapeSlider.getBounds().toString()
                         + " phase=" + wobblePhaseSlider.getBounds().toString()
                         + " mix=" + wobbleMixSlider.getBounds().toString()
                         + " mode=" + wobbleModeBox.getBounds().toString()
                         + " bypass=" + wobbleBypassButton.getBounds().toString());
    }

    // Width
    widthPanel.setBounds (x0 + 6 * (panelWidth + gap), rects.modules.getY(), panelWidth, rects.modules.getHeight());
    {
        auto c = widthPanel.getContentBounds();
        widthSlider.setBounds (toEditorBounds (widthPanel, placeLarge (c, 76)));
        widthLowCutSlider.setBounds (toEditorBounds (widthPanel, placeSmall (c, 0, 54, 54, 96)));
        widthBalanceSlider.setBounds (toEditorBounds (widthPanel, placeSmall (c, 1, 54, 54, 96)));
        widthHighCutSlider.setBounds (toEditorBounds (widthPanel, placeSmall (c, 0, 54, 54, 170)));
        widthMonoBelowButton.setBounds (toEditorBounds (widthPanel, placeWide (c, 196, 26)));
        widthBypassButton.setBounds (toEditorBounds (widthPanel, placeWide (c, 228, 26)));
        appendDebugLine ("widthPanel=" + widthPanel.getBounds().toString() + " content=" + c.toString()
                         + " width=" + widthSlider.getBounds().toString()
                         + " lowCut=" + widthLowCutSlider.getBounds().toString()
                         + " balance=" + widthBalanceSlider.getBounds().toString()
                         + " highCut=" + widthHighCutSlider.getBounds().toString()
                         + " monoBelow=" + widthMonoBelowButton.getBounds().toString()
                         + " bypass=" + widthBypassButton.getBounds().toString());
    }

    // Output
    outputPanel.setBounds (x0 + 7 * (panelWidth + gap), rects.modules.getY(), panelWidth, rects.modules.getHeight());
    {
        auto c = outputPanel.getContentBounds();
        outputGainSlider.setBounds (toEditorBounds (outputPanel, placeLarge (c, 80)));
        safetyCeilingSlider.setBounds (toEditorBounds (outputPanel, placeSmall (c, 0, 58, 58, 98)));
        limiterThresholdSlider.setBounds (toEditorBounds (outputPanel, placeSmall (c, 1, 58, 58, 98)));
        limiterReleaseSlider.setBounds (toEditorBounds (outputPanel, placeSmall (c, 0, 58, 58, 172)));
        softClipSlider.setBounds (toEditorBounds (outputPanel, placeSmall (c, 1, 58, 58, 172)));
        panicMuteButton.setBounds (toEditorBounds (outputPanel, placeWide (c, 264, 26)));
        appendDebugLine ("outputPanel=" + outputPanel.getBounds().toString() + " content=" + c.toString()
                         + " gain=" + outputGainSlider.getBounds().toString()
                         + " ceiling=" + safetyCeilingSlider.getBounds().toString()
                         + " threshold=" + limiterThresholdSlider.getBounds().toString()
                         + " release=" + limiterReleaseSlider.getBounds().toString()
                         + " softClip=" + softClipSlider.getBounds().toString()
                         + " panic=" + panicMuteButton.getBounds().toString());
    }

    safeModePanel.setBounds (rects.safeMode);

    auto meters = rects.meters.reduced (16, 18);
    auto meterCol = meters.removeFromLeft (64);
    inputMeter.setBounds (meterCol);
    meters.removeFromLeft (8);
    meterCol = meters.removeFromLeft (64);
    outputMeter.setBounds (meterCol);
    meters.removeFromLeft (8);
    reductionMeter.setBounds (meters.removeFromLeft (84));

    appendDebugLine ("safeMode=" + safeModePanel.getBounds().toString()
                     + " inputMeter=" + inputMeter.getBounds().toString()
                     + " outputMeter=" + outputMeter.getBounds().toString()
                     + " reductionMeter=" + reductionMeter.getBounds().toString());
}

void SafeToolboxAudioProcessorEditor::timerCallback()
{
    presetBar.refreshPresets();
    inputMeter.repaint();
    outputMeter.repaint();
    reductionMeter.repaint();
    safeModePanel.repaint();
}

void SafeToolboxAudioProcessorEditor::configureSlider (juce::Slider& slider,
                                                        const juce::String& tooltip,
                                                        juce::Colour accent,
                                                        RotaryKnobLookAndFeel& laf)
{
    addAndMakeVisible (slider);
    slider.setName (tooltip);
    laf.setAccentColour (accent);
    slider.setLookAndFeel (&laf);
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    slider.setRotaryParameters (juce::MathConstants<float>::pi * 0.74f,
                                juce::MathConstants<float>::pi * 2.26f,
                                true);
    slider.setColour (juce::Slider::backgroundColourId, juce::Colours::transparentBlack);
    slider.setTooltip (tooltip);
}

void SafeToolboxAudioProcessorEditor::configureComboBox (juce::ComboBox& comboBox, const juce::String& tooltip, const juce::StringArray& items)
{
    addAndMakeVisible (comboBox);
    comboBox.setName (tooltip);
    comboBox.clear (juce::dontSendNotification);

    for (int i = 0; i < items.size(); ++i)
        comboBox.addItem (items[i], i + 1);

    comboBox.setTooltip (tooltip);
    comboBox.setColour (juce::ComboBox::backgroundColourId, juce::Colour (0xaa11151c));
    comboBox.setColour (juce::ComboBox::textColourId, juce::Colours::white.withAlpha (0.92f));
    comboBox.setColour (juce::ComboBox::arrowColourId, juce::Colours::white.withAlpha (0.8f));
    comboBox.setColour (juce::ComboBox::outlineColourId, juce::Colour (0xff2b313b));
}

void SafeToolboxAudioProcessorEditor::configureButton (juce::TextButton& button, const juce::String& tooltip, juce::Colour offColour, juce::Colour onColour)
{
    addAndMakeVisible (button);
    button.setName (tooltip);
    button.setTooltip (tooltip);
    button.setClickingTogglesState (true);
    button.setColour (juce::TextButton::buttonColourId, offColour);
    button.setColour (juce::TextButton::buttonOnColourId, onColour);
    button.setColour (juce::TextButton::textColourOffId, juce::Colours::white.withAlpha (0.90f));
    button.setColour (juce::TextButton::textColourOnId, juce::Colours::white);
}

void SafeToolboxAudioProcessorEditor::logDebugLayout (const juce::String& tag)
{
#if SAFE_TOOLBOX_DEBUG_UI
    appendDebugLine (tag + " desktopScale=" + juce::String (juce::Desktop::getInstance().getGlobalScaleFactor(), 2)
                     + " editor=" + getBounds().toString()
                     + " local=" + getLocalBounds().toString());
#else
    juce::ignoreUnused (tag);
#endif
}

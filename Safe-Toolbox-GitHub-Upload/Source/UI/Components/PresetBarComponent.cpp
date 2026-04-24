#include "PresetBarComponent.h"

#include "../LookAndFeel/Theme.h"

namespace
{
    void configureComboBox (juce::ComboBox& comboBox, const juce::String& tooltip, const juce::StringArray& items)
    {
        comboBox.clear (juce::dontSendNotification);

        for (int i = 0; i < items.size(); ++i)
            comboBox.addItem (items[i], i + 1);

        comboBox.setTooltip (tooltip);
        comboBox.setColour (juce::ComboBox::backgroundColourId, juce::Colour (0xaa11151c));
        comboBox.setColour (juce::ComboBox::textColourId, juce::Colours::white.withAlpha (0.92f));
        comboBox.setColour (juce::ComboBox::arrowColourId, juce::Colours::white.withAlpha (0.8f));
        comboBox.setColour (juce::ComboBox::outlineColourId, juce::Colour (0xff2b313b));
    }

    void styleButton (juce::TextButton& button)
    {
        button.setClickingTogglesState (false);
        button.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff23272f));
        button.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff5f6a7a));
        button.setColour (juce::TextButton::textColourOffId, juce::Colours::white.withAlpha (0.90f));
        button.setColour (juce::TextButton::textColourOnId, juce::Colours::white);
    }
}

PresetBarComponent::PresetBarComponent (SafeToolboxAudioProcessor& p)
    : audioProcessor (p)
{
    setOpaque (true);

    addAndMakeVisible (presetBox);
    configureComboBox (presetBox, "Preset selection", SafeToolboxAudioProcessor::getPresetNames());
    presetBox.onChange = [this]
    {
        audioProcessor.setCurrentProgram (presetBox.getSelectedItemIndex());
    };

    addAndMakeVisible (aButton);
    addAndMakeVisible (bButton);
    addAndMakeVisible (copyButton);
    addAndMakeVisible (pasteButton);
    addAndMakeVisible (compareButton);

    styleButton (aButton);
    styleButton (bButton);
    styleButton (copyButton);
    styleButton (pasteButton);
    styleButton (compareButton);

    refreshPresets();
}

void PresetBarComponent::refreshPresets()
{
    const auto names = SafeToolboxAudioProcessor::getPresetNames();
    const auto currentIndex = juce::jlimit (0, juce::jmax (0, names.size() - 1), audioProcessor.getCurrentProgram());

    if (presetBox.getNumItems() != names.size())
    {
        presetBox.clear (juce::dontSendNotification);

        for (int i = 0; i < names.size(); ++i)
            presetBox.addItem (names[i], i + 1);
    }

    if (presetBox.getSelectedItemIndex() != currentIndex)
        presetBox.setSelectedItemIndex (currentIndex, juce::dontSendNotification);
}

void PresetBarComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (1.0f);

    g.setColour (SafeToolboxTheme::panelFill());
    g.fillRoundedRectangle (bounds, 16.0f);
    g.setColour (SafeToolboxTheme::panelOutline());
    g.drawRoundedRectangle (bounds, 16.0f, 1.0f);

    auto labelArea = getLocalBounds().reduced (16, 0).removeFromTop (20);
    g.setColour (SafeToolboxTheme::mutedText().withAlpha (0.86f));
    g.setFont (juce::FontOptions (9.2f, juce::Font::bold));
    g.drawText ("PRESETS", labelArea.reduced (2, 0), juce::Justification::centredLeft);

    g.setColour (SafeToolboxTheme::panelOutline().withAlpha (0.65f));
    g.fillRect (labelArea.getX(), labelArea.getBottom() - 1, labelArea.getWidth(), 1);
}

void PresetBarComponent::resized()
{
    auto area = getLocalBounds().reduced (14, 8);
    area.removeFromTop (18);
    auto buttonArea = area.removeFromRight (284);
    presetBox.setBounds (area.removeFromLeft (282).withHeight (28));

    aButton.setBounds (buttonArea.removeFromLeft (28));
    buttonArea.removeFromLeft (8);
    bButton.setBounds (buttonArea.removeFromLeft (28));
    buttonArea.removeFromLeft (8);
    copyButton.setBounds (buttonArea.removeFromLeft (54));
    buttonArea.removeFromLeft (8);
    pasteButton.setBounds (buttonArea.removeFromLeft (54));
    buttonArea.removeFromLeft (8);
    compareButton.setBounds (buttonArea.removeFromLeft (72));
}

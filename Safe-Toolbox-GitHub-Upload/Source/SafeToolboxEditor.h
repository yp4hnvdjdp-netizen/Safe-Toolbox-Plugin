#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "SafeToolboxProcessor.h"
#include "UI/Components/HeaderComponent.h"
#include "UI/Components/MeterComponent.h"
#include "UI/Components/ModulePanel.h"
#include "UI/Components/PresetBarComponent.h"
#include "UI/Components/SafeModePanel.h"
#include "UI/Assets/AssetIds.h"
#include "UI/Assets/AssetLoader.h"
#include "UI/LookAndFeel/RotaryKnobLookAndFeel.h"
#include "UI/LookAndFeel/SafeToolboxLookAndFeel.h"

class SafeToolboxAudioProcessorEditor : public juce::AudioProcessorEditor,
                                        private juce::Timer
{
public:
    explicit SafeToolboxAudioProcessorEditor (SafeToolboxAudioProcessor&);
    ~SafeToolboxAudioProcessorEditor() override;

    void paint (juce::Graphics& g) override;
    void paintOverChildren (juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;
    void configureSlider (juce::Slider& slider, const juce::String& tooltip, juce::Colour accent, RotaryKnobLookAndFeel& laf);
    void configureComboBox (juce::ComboBox& comboBox, const juce::String& tooltip, const juce::StringArray& items);
    void configureButton (juce::TextButton& button, const juce::String& tooltip, juce::Colour offColour, juce::Colour onColour);
    void logDebugLayout (const juce::String& tag);

    SafeToolboxAudioProcessor& audioProcessor;
    HeaderComponent headerComponent;
    PresetBarComponent presetBar;
    ModulePanel inputPanel;
    ModulePanel drivePanel;
    ModulePanel filterPanel;
    ModulePanel delayPanel;
    ModulePanel reverbPanel;
    ModulePanel wobblePanel;
    ModulePanel widthPanel;
    ModulePanel outputPanel;
    SafeModePanel safeModePanel;
    MeterComponent inputMeter;
    MeterComponent outputMeter;
    MeterComponent reductionMeter;

    juce::Slider inputGainSlider;

    juce::Slider driveAmountSlider;
    juce::Slider driveToneSlider;
    juce::Slider driveMixSlider;
    juce::Slider driveOutputSlider;
    juce::TextButton driveBypassButton { "Bypass" };

    juce::ComboBox filterTypeBox;
    juce::Slider filterCutoffSlider;
    juce::Slider filterResonanceSlider;
    juce::Slider filterDriveSlider;
    juce::ComboBox filterSlopeBox;
    juce::TextButton filterBypassButton { "Bypass" };

    juce::ComboBox delayModeBox;
    juce::ComboBox delaySyncBox;
    juce::Slider delayTimeSlider;
    juce::Slider delayFeedbackSlider;
    juce::Slider delayDampingSlider;
    juce::Slider delayMixSlider;
    juce::TextButton delayBypassButton { "Bypass" };

    juce::ComboBox reverbModeBox;
    juce::Slider reverbSizeSlider;
    juce::Slider reverbDecaySlider;
    juce::Slider reverbDampingSlider;
    juce::Slider reverbWidthSlider;
    juce::Slider reverbMixSlider;
    juce::TextButton reverbBypassButton { "Bypass" };

    juce::ComboBox wobbleModeBox;
    juce::Slider wobbleRateSlider;
    juce::Slider wobbleDepthSlider;
    juce::Slider wobbleShapeSlider;
    juce::Slider wobblePhaseSlider;
    juce::Slider wobbleMixSlider;
    juce::TextButton wobbleBypassButton { "Bypass" };

    juce::Slider widthSlider;
    juce::Slider widthLowCutSlider;
    juce::Slider widthBalanceSlider;
    juce::Slider widthHighCutSlider;
    juce::TextButton widthMonoBelowButton { "Mono Below" };
    juce::TextButton widthBypassButton { "Bypass" };

    juce::Slider outputGainSlider;
    juce::Slider safetyCeilingSlider;
    juce::Slider limiterThresholdSlider;
    juce::Slider limiterReleaseSlider;
    juce::Slider softClipSlider;
    juce::TextButton panicMuteButton { "Panic Mute" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAmountAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveToneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveOutputAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> driveBypassAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterDriveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterSlopeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> filterBypassAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> delayModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> delaySyncAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayFeedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayDampingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> delayBypassAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> reverbModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbSizeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbDecayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbDampingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbWidthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> reverbBypassAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> wobbleModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wobbleRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wobbleDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wobbleShapeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wobblePhaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wobbleMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> wobbleBypassAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthLowCutAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthBalanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthHighCutAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> widthMonoBelowAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> widthBypassAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> safetyCeilingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> limiterThresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> limiterReleaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> softClipAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> panicMuteAttachment;

    SafeToolboxLookAndFeel uiLaf;
    RotaryKnobLookAndFeel neutralLaf;
    RotaryKnobLookAndFeel driveLaf;
    RotaryKnobLookAndFeel driveSupportLaf;
    RotaryKnobLookAndFeel filterLaf;
    RotaryKnobLookAndFeel filterSupportLaf;
    RotaryKnobLookAndFeel delayLaf;
    RotaryKnobLookAndFeel delaySupportLaf;
    RotaryKnobLookAndFeel reverbLaf;
    RotaryKnobLookAndFeel reverbSupportLaf;
    RotaryKnobLookAndFeel wobbleLaf;
    RotaryKnobLookAndFeel widthLaf;
    RotaryKnobLookAndFeel outputLaf;
    juce::Image backgroundArtwork;
    std::unique_ptr<juce::Drawable> backdropTexture;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SafeToolboxAudioProcessorEditor)
};

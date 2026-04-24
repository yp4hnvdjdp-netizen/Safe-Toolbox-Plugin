#include "ParameterLayout.h"

#include "ParameterIDs.h"

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    using APF = juce::AudioParameterFloat;
    using APC = juce::AudioParameterChoice;
    using APB = juce::AudioParameterBool;

    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::inputGain, SafeToolboxParamIDs::currentParameterVersion },
                                       "Input Gain",
                                       juce::NormalisableRange<float> (-24.0f, 24.0f, 0.01f),
                                       0.0f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::safetyCeiling, SafeToolboxParamIDs::currentParameterVersion },
                                       "Safety Ceiling",
                                       juce::NormalisableRange<float> (-12.0f, -0.1f, 0.01f),
                                       -0.5f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::limiterThreshold, SafeToolboxParamIDs::currentParameterVersion },
                                       "Limiter Threshold",
                                       juce::NormalisableRange<float> (-18.0f, -0.1f, 0.01f),
                                       -1.0f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::limiterRelease, SafeToolboxParamIDs::currentParameterVersion },
                                       "Limiter Release",
                                       juce::NormalisableRange<float> (5.0f, 500.0f, 0.01f, 0.5f),
                                       80.0f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::softClip, SafeToolboxParamIDs::currentParameterVersion },
                                       "Soft Clip",
                                       juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
                                       0.0f));
    layout.add (std::make_unique<APB> (juce::ParameterID { SafeToolboxParamIDs::panicMute, SafeToolboxParamIDs::currentParameterVersion },
                                       "Panic Mute",
                                       false));

    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::driveAmount, SafeToolboxParamIDs::currentParameterVersion },
                                       "Drive Amount",
                                       juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 0.5f),
                                       0.25f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::driveTone, SafeToolboxParamIDs::currentParameterVersion },
                                       "Drive Tone",
                                       juce::NormalisableRange<float> (80.0f, 20000.0f, 0.01f, 0.5f),
                                       20000.0f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::driveMix, SafeToolboxParamIDs::currentParameterVersion },
                                       "Drive Mix",
                                       juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
                                       0.0f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::driveBias, SafeToolboxParamIDs::currentParameterVersion },
                                       "Drive Bias",
                                       juce::NormalisableRange<float> (-1.0f, 1.0f, 0.001f),
                                       0.0f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::driveOutput, SafeToolboxParamIDs::currentParameterVersion },
                                       "Drive Output",
                                       juce::NormalisableRange<float> (-24.0f, 24.0f, 0.01f),
                                       0.0f));
    layout.add (std::make_unique<APB> (juce::ParameterID { SafeToolboxParamIDs::driveBypass, SafeToolboxParamIDs::currentParameterVersion },
                                       "Drive Bypass",
                                       false));

    layout.add (std::make_unique<APC> (juce::ParameterID { SafeToolboxParamIDs::filterType, SafeToolboxParamIDs::currentParameterVersion },
                                       "Filter Type",
                                       SafeToolboxParamIDs::filterTypeChoices(),
                                       1));
    layout.add (std::make_unique<APC> (juce::ParameterID { SafeToolboxParamIDs::filterSlope, SafeToolboxParamIDs::currentParameterVersion },
                                       "Filter Slope",
                                       SafeToolboxParamIDs::filterSlopeChoices(),
                                       0));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::filterCutoff, SafeToolboxParamIDs::currentParameterVersion },
                                       "Filter Cutoff",
                                       juce::NormalisableRange<float> (20.0f, 20000.0f, 0.01f, 0.5f),
                                       12000.0f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::filterResonance, SafeToolboxParamIDs::currentParameterVersion },
                                       "Filter Resonance",
                                       juce::NormalisableRange<float> (0.2f, 12.0f, 0.001f, 0.5f),
                                       0.7f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::filterDrive, SafeToolboxParamIDs::currentParameterVersion },
                                       "Filter Drive",
                                       juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
                                       0.0f));
    layout.add (std::make_unique<APB> (juce::ParameterID { SafeToolboxParamIDs::filterBypass, SafeToolboxParamIDs::currentParameterVersion },
                                       "Filter Bypass",
                                       false));

    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::delayTime, SafeToolboxParamIDs::currentParameterVersion },
                                       "Delay Time",
                                       juce::NormalisableRange<float> (0.01f, 2.0f, 0.001f, 0.5f),
                                       0.18f));
    layout.add (std::make_unique<APC> (juce::ParameterID { SafeToolboxParamIDs::delaySync, SafeToolboxParamIDs::currentParameterVersion },
                                       "Delay Sync",
                                       SafeToolboxParamIDs::delaySyncChoices(),
                                       0));
    layout.add (std::make_unique<APC> (juce::ParameterID { SafeToolboxParamIDs::delayMode, SafeToolboxParamIDs::currentParameterVersion },
                                       "Delay Mode",
                                       SafeToolboxParamIDs::delayModeChoices(),
                                       0));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::delayFeedback, SafeToolboxParamIDs::currentParameterVersion },
                                       "Delay Feedback",
                                       juce::NormalisableRange<float> (0.0f, 0.65f, 0.001f),
                                       0.25f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::delayMix, SafeToolboxParamIDs::currentParameterVersion },
                                       "Delay Mix",
                                       juce::NormalisableRange<float> (0.0f, 0.5f, 0.001f),
                                       0.25f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::delayFeedbackTone, SafeToolboxParamIDs::currentParameterVersion },
                                       "Delay Feedback Tone",
                                       juce::NormalisableRange<float> (80.0f, 20000.0f, 0.01f, 0.5f),
                                       6000.0f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::delayStereoOffset, SafeToolboxParamIDs::currentParameterVersion },
                                       "Delay Stereo Offset",
                                       juce::NormalisableRange<float> (0.0f, 50.0f, 0.01f, 0.5f),
                                       0.0f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::delayDamping, SafeToolboxParamIDs::currentParameterVersion },
                                       "Delay Damping",
                                       juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
                                       0.35f));
    layout.add (std::make_unique<APB> (juce::ParameterID { SafeToolboxParamIDs::delayBypass, SafeToolboxParamIDs::currentParameterVersion },
                                       "Delay Bypass",
                                       false));

    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::reverbSize, SafeToolboxParamIDs::currentParameterVersion },
                                       "Reverb Size",
                                       juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
                                       0.35f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::reverbDecay, SafeToolboxParamIDs::currentParameterVersion },
                                       "Reverb Decay",
                                       juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
                                       0.35f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::reverbDamping, SafeToolboxParamIDs::currentParameterVersion },
                                       "Reverb Damping",
                                       juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
                                       0.35f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::reverbPreDelay, SafeToolboxParamIDs::currentParameterVersion },
                                       "Reverb Pre-Delay",
                                       juce::NormalisableRange<float> (0.0f, 250.0f, 0.01f, 0.5f),
                                       0.0f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::reverbMix, SafeToolboxParamIDs::currentParameterVersion },
                                       "Reverb Mix",
                                       juce::NormalisableRange<float> (0.0f, 0.5f, 0.001f),
                                       0.18f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::reverbTone, SafeToolboxParamIDs::currentParameterVersion },
                                       "Reverb Tone",
                                       juce::NormalisableRange<float> (120.0f, 20000.0f, 0.01f, 0.5f),
                                       9000.0f));
    layout.add (std::make_unique<APC> (juce::ParameterID { SafeToolboxParamIDs::reverbMode, SafeToolboxParamIDs::currentParameterVersion },
                                       "Reverb Mode",
                                       SafeToolboxParamIDs::reverbModeChoices(),
                                       0));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::reverbWidth, SafeToolboxParamIDs::currentParameterVersion },
                                       "Reverb Width",
                                       juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
                                       1.0f));
    layout.add (std::make_unique<APB> (juce::ParameterID { SafeToolboxParamIDs::reverbBypass, SafeToolboxParamIDs::currentParameterVersion },
                                       "Reverb Bypass",
                                       false));

    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::wobbleRate, SafeToolboxParamIDs::currentParameterVersion },
                                       "Wobble Rate",
                                       juce::NormalisableRange<float> (0.05f, 8.0f, 0.001f, 0.5f),
                                       0.2f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::wobbleDepth, SafeToolboxParamIDs::currentParameterVersion },
                                       "Wobble Depth",
                                       juce::NormalisableRange<float> (0.0f, 12.0f, 0.001f, 0.5f),
                                       0.4f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::wobbleShape, SafeToolboxParamIDs::currentParameterVersion },
                                       "Wobble Shape",
                                       juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
                                       0.5f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::wobbleStereoPhase, SafeToolboxParamIDs::currentParameterVersion },
                                       "Wobble Stereo Phase",
                                       juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
                                       0.5f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::wobbleMix, SafeToolboxParamIDs::currentParameterVersion },
                                       "Wobble Mix",
                                       juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
                                       0.25f));
    layout.add (std::make_unique<APC> (juce::ParameterID { SafeToolboxParamIDs::wobbleMode, SafeToolboxParamIDs::currentParameterVersion },
                                       "Wobble Mode",
                                       SafeToolboxParamIDs::wobbleModeChoices(),
                                       1));
    layout.add (std::make_unique<APB> (juce::ParameterID { SafeToolboxParamIDs::wobbleBypass, SafeToolboxParamIDs::currentParameterVersion },
                                       "Wobble Bypass",
                                       false));

    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::width, SafeToolboxParamIDs::currentParameterVersion },
                                       "Width",
                                       juce::NormalisableRange<float> (0.0f, 1.5f, 0.001f),
                                       1.0f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::widthLowCut, SafeToolboxParamIDs::currentParameterVersion },
                                       "Width Low Cut",
                                       juce::NormalisableRange<float> (20.0f, 1000.0f, 0.01f, 0.5f),
                                       120.0f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::widthHighCut, SafeToolboxParamIDs::currentParameterVersion },
                                       "Width High Cut",
                                       juce::NormalisableRange<float> (1000.0f, 20000.0f, 0.01f, 0.5f),
                                       12000.0f));
    layout.add (std::make_unique<APB> (juce::ParameterID { SafeToolboxParamIDs::widthMonoBelow, SafeToolboxParamIDs::currentParameterVersion },
                                       "Width Mono Below",
                                       false));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::balance, SafeToolboxParamIDs::currentParameterVersion },
                                       "Balance",
                                       juce::NormalisableRange<float> (-1.0f, 1.0f, 0.001f),
                                       0.0f));
    layout.add (std::make_unique<APB> (juce::ParameterID { SafeToolboxParamIDs::widthBypass, SafeToolboxParamIDs::currentParameterVersion },
                                       "Width Bypass",
                                       false));

    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::outputGain, SafeToolboxParamIDs::currentParameterVersion },
                                       "Output Gain",
                                       juce::NormalisableRange<float> (-24.0f, 24.0f, 0.01f),
                                       0.0f));
    layout.add (std::make_unique<APF> (juce::ParameterID { SafeToolboxParamIDs::globalMix, SafeToolboxParamIDs::currentParameterVersion },
                                       "Global Mix",
                                       juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
                                       1.0f));
    layout.add (std::make_unique<APB> (juce::ParameterID { SafeToolboxParamIDs::bypass, SafeToolboxParamIDs::currentParameterVersion },
                                       "Bypass",
                                       false));

    return layout;
}

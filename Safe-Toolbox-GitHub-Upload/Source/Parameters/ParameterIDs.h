#pragma once

#include <juce_core/juce_core.h>

namespace SafeToolboxParamIDs
{
    inline constexpr auto inputGain = "inputGain";
    inline constexpr auto safetyCeiling = "safetyCeiling";
    inline constexpr auto limiterThreshold = "limiterThreshold";
    inline constexpr auto limiterRelease = "limiterRelease";
    inline constexpr auto softClip = "softClip";
    inline constexpr auto panicMute = "panicMute";

    inline constexpr auto driveAmount = "driveAmount";
    inline constexpr auto driveTone = "driveTone";
    inline constexpr auto driveMix = "driveMix";
    inline constexpr auto driveBias = "driveBias";
    inline constexpr auto driveOutput = "driveOutput";
    inline constexpr auto driveBypass = "driveBypass";

    inline constexpr auto filterType = "filterType";
    inline constexpr auto filterSlope = "filterSlope";
    inline constexpr auto filterCutoff = "filterCutoff";
    inline constexpr auto filterResonance = "filterResonance";
    inline constexpr auto filterDrive = "filterDrive";
    inline constexpr auto filterBypass = "filterBypass";

    inline constexpr auto delayTime = "delayTime";
    inline constexpr auto delaySync = "delaySync";
    inline constexpr auto delayMode = "delayMode";
    inline constexpr auto delayFeedback = "delayFeedback";
    inline constexpr auto delayMix = "delayMix";
    inline constexpr auto delayFeedbackTone = "delayFeedbackTone";
    inline constexpr auto delayStereoOffset = "delayStereoOffset";
    inline constexpr auto delayDamping = "delayDamping";
    inline constexpr auto delayBypass = "delayBypass";

    inline constexpr auto reverbSize = "reverbSize";
    inline constexpr auto reverbDecay = "reverbDecay";
    inline constexpr auto reverbDamping = "reverbDamping";
    inline constexpr auto reverbPreDelay = "reverbPreDelay";
    inline constexpr auto reverbMix = "reverbMix";
    inline constexpr auto reverbTone = "reverbTone";
    inline constexpr auto reverbMode = "reverbMode";
    inline constexpr auto reverbWidth = "reverbWidth";
    inline constexpr auto reverbBypass = "reverbBypass";

    inline constexpr auto wobbleRate = "wobbleRate";
    inline constexpr auto wobbleDepth = "wobbleDepth";
    inline constexpr auto wobbleRandomness = "wobbleRandomness";
    inline constexpr auto wobbleStereoPhase = "wobbleStereoPhase";
    inline constexpr auto wobbleSmoothing = "wobbleSmoothing";
    inline constexpr auto wobbleMode = "wobbleMode";
    inline constexpr auto wobbleShape = "wobbleShape";
    inline constexpr auto wobbleMix = "wobbleMix";
    inline constexpr auto wobbleBypass = "wobbleBypass";

    inline constexpr auto width = "width";
    inline constexpr auto widthLowCut = "widthLowCut";
    inline constexpr auto widthHighCut = "widthHighCut";
    inline constexpr auto widthMonoBelow = "widthMonoBelow";
    inline constexpr auto balance = "balance";
    inline constexpr auto widthBypass = "widthBypass";

    inline constexpr auto outputGain = "outputGain";
    inline constexpr auto globalMix = "globalMix";
    inline constexpr auto bypass = "bypass";

    inline constexpr int currentParameterVersion = 1;

    inline juce::StringArray filterTypeChoices()
    {
        return { "Off", "Lowpass", "Highpass", "Bandpass", "Notch" };
    }

    inline juce::StringArray filterSlopeChoices()
    {
        return { "12 dB", "24 dB" };
    }

    inline juce::StringArray delaySyncChoices()
    {
        return { "Off", "1/4", "1/8", "1/8D", "1/16", "1 1/2" };
    }

    inline juce::StringArray delayModeChoices()
    {
        return { "Stereo", "Ping Pong" };
    }

    inline juce::StringArray reverbModeChoices()
    {
        return { "Plate", "Room", "Hall" };
    }

    inline juce::StringArray wobbleModeChoices()
    {
        return { "Off", "Subtle", "Chorus", "Tape" };
    }
}

#include "PresetDefinitions.h"

#include "ParameterIDs.h"
#include "../SafeToolboxProcessor.h"

namespace
{
    void setParameterValue (SafeToolboxAudioProcessor& processor, const juce::String& parameterId, float value)
    {
        if (auto* parameter = dynamic_cast<juce::RangedAudioParameter*> (processor.apvts.getParameter (parameterId)))
            parameter->setValueNotifyingHost (parameter->convertTo0to1 (value));
    }

    const std::vector<PresetData>& presetList()
    {
        static const std::vector<PresetData> presets
        {
            { "Clean Safe", [] (SafeToolboxAudioProcessor& p)
                {
                    setParameterValue (p, SafeToolboxParamIDs::inputGain, 0.0f);
                    setParameterValue (p, SafeToolboxParamIDs::outputGain, 0.0f);
                    setParameterValue (p, SafeToolboxParamIDs::driveAmount, 0.0f);
                    setParameterValue (p, SafeToolboxParamIDs::driveMix, 0.0f);
                    setParameterValue (p, SafeToolboxParamIDs::driveOutput, 0.0f);
                    setParameterValue (p, SafeToolboxParamIDs::filterType, 1.0f);
                    setParameterValue (p, SafeToolboxParamIDs::filterCutoff, 16000.0f);
                    setParameterValue (p, SafeToolboxParamIDs::delayMode, 0.0f);
                    setParameterValue (p, SafeToolboxParamIDs::delayMix, 0.08f);
                    setParameterValue (p, SafeToolboxParamIDs::delayDamping, 0.35f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbMix, 0.08f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbMode, 0.0f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbWidth, 1.0f);
                    setParameterValue (p, SafeToolboxParamIDs::wobbleDepth, 0.10f);
                    setParameterValue (p, SafeToolboxParamIDs::wobbleMode, 1.0f);
                    setParameterValue (p, SafeToolboxParamIDs::wobbleShape, 0.5f);
                    setParameterValue (p, SafeToolboxParamIDs::wobbleMix, 0.25f);
                    setParameterValue (p, SafeToolboxParamIDs::width, 1.0f);
                    setParameterValue (p, SafeToolboxParamIDs::widthMonoBelow, 0.0f);
                    setParameterValue (p, SafeToolboxParamIDs::globalMix, 1.0f);
                    setParameterValue (p, SafeToolboxParamIDs::bypass, 0.0f);
                } },
            { "Warm Glue", [] (SafeToolboxAudioProcessor& p)
                {
                    setParameterValue (p, SafeToolboxParamIDs::driveAmount, 0.25f);
                    setParameterValue (p, SafeToolboxParamIDs::driveMix, 0.35f);
                    setParameterValue (p, SafeToolboxParamIDs::driveTone, 6000.0f);
                    setParameterValue (p, SafeToolboxParamIDs::driveOutput, -1.5f);
                    setParameterValue (p, SafeToolboxParamIDs::filterType, 1.0f);
                    setParameterValue (p, SafeToolboxParamIDs::filterCutoff, 9000.0f);
                    setParameterValue (p, SafeToolboxParamIDs::filterResonance, 0.9f);
                    setParameterValue (p, SafeToolboxParamIDs::delayMode, 0.0f);
                    setParameterValue (p, SafeToolboxParamIDs::delayMix, 0.15f);
                    setParameterValue (p, SafeToolboxParamIDs::delayDamping, 0.45f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbMix, 0.18f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbSize, 0.28f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbWidth, 0.9f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbMode, 0.0f);
                    setParameterValue (p, SafeToolboxParamIDs::globalMix, 1.0f);
                } },
            { "Dub Bloom", [] (SafeToolboxAudioProcessor& p)
                {
                    setParameterValue (p, SafeToolboxParamIDs::delayTime, 0.42f);
                    setParameterValue (p, SafeToolboxParamIDs::delayFeedback, 0.52f);
                    setParameterValue (p, SafeToolboxParamIDs::delayMix, 0.30f);
                    setParameterValue (p, SafeToolboxParamIDs::delayMode, 1.0f);
                    setParameterValue (p, SafeToolboxParamIDs::delayFeedbackTone, 3200.0f);
                    setParameterValue (p, SafeToolboxParamIDs::delayDamping, 0.62f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbMix, 0.24f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbDecay, 0.58f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbSize, 0.62f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbWidth, 1.0f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbMode, 2.0f);
                    setParameterValue (p, SafeToolboxParamIDs::wobbleDepth, 0.25f);
                    setParameterValue (p, SafeToolboxParamIDs::wobbleMode, 2.0f);
                    setParameterValue (p, SafeToolboxParamIDs::wobbleShape, 0.7f);
                    setParameterValue (p, SafeToolboxParamIDs::wobbleMix, 0.3f);
                    setParameterValue (p, SafeToolboxParamIDs::width, 1.12f);
                } },
            { "Wobble Drift", [] (SafeToolboxAudioProcessor& p)
                {
                    setParameterValue (p, SafeToolboxParamIDs::wobbleRate, 0.75f);
                    setParameterValue (p, SafeToolboxParamIDs::wobbleDepth, 1.6f);
                    setParameterValue (p, SafeToolboxParamIDs::wobbleStereoPhase, 0.75f);
                    setParameterValue (p, SafeToolboxParamIDs::wobbleShape, 0.85f);
                    setParameterValue (p, SafeToolboxParamIDs::wobbleMix, 0.25f);
                    setParameterValue (p, SafeToolboxParamIDs::wobbleMode, 3.0f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbMix, 0.10f);
                    setParameterValue (p, SafeToolboxParamIDs::delayMix, 0.12f);
                    setParameterValue (p, SafeToolboxParamIDs::widthMonoBelow, 1.0f);
                } },
            { "Wide Plate", [] (SafeToolboxAudioProcessor& p)
                {
                    setParameterValue (p, SafeToolboxParamIDs::reverbMix, 0.30f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbSize, 0.78f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbDecay, 0.55f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbDamping, 0.42f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbWidth, 1.0f);
                    setParameterValue (p, SafeToolboxParamIDs::reverbMode, 2.0f);
                    setParameterValue (p, SafeToolboxParamIDs::width, 1.30f);
                    setParameterValue (p, SafeToolboxParamIDs::widthHighCut, 16000.0f);
                    setParameterValue (p, SafeToolboxParamIDs::balance, 0.0f);
                } },
            { "Narrow Safe", [] (SafeToolboxAudioProcessor& p)
                {
                    setParameterValue (p, SafeToolboxParamIDs::width, 0.25f);
                    setParameterValue (p, SafeToolboxParamIDs::widthMonoBelow, 1.0f);
                    setParameterValue (p, SafeToolboxParamIDs::filterType, 2.0f);
                    setParameterValue (p, SafeToolboxParamIDs::filterCutoff, 260.0f);
                    setParameterValue (p, SafeToolboxParamIDs::delayMix, 0.08f);
                } }
        };

        return presets;
    }
}

juce::StringArray getPresetNames()
{
    juce::StringArray names;

    for (const auto& preset : presetList())
        names.add (preset.name);

    return names;
}

const std::vector<PresetData>& getPresets()
{
    return presetList();
}

void applyPreset (SafeToolboxAudioProcessor& processor, int index)
{
    const auto& presets = presetList();

    if (! juce::isPositiveAndBelow (index, static_cast<int> (presets.size())))
        return;

    presets[(size_t) index].apply (processor);
}

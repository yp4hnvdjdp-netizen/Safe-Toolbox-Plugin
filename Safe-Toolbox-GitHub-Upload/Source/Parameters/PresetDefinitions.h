#pragma once

#include <functional>
#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>

class SafeToolboxAudioProcessor;

struct PresetData
{
    juce::String name;
    std::function<void (SafeToolboxAudioProcessor&)> apply;
};

juce::StringArray getPresetNames();
const std::vector<PresetData>& getPresets();
void applyPreset (SafeToolboxAudioProcessor& processor, int index);

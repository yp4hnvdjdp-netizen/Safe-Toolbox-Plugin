#pragma once

#include <atomic>
#include <juce_audio_processors/juce_audio_processors.h>

#include "DSP/MeterState.h"
#include "DSP/SignalChain.h"
#include "Parameters/ParameterLayout.h"
#include "Parameters/ParameterSnapshot.h"

class SafeToolboxAudioProcessorEditor;

class SafeToolboxAudioProcessor : public juce::AudioProcessor
{
public:
    SafeToolboxAudioProcessor();
    ~SafeToolboxAudioProcessor() override = default;

    const juce::String getName() const override { return "Safe Toolbox"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 6.0; }

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void prepareToPlay (double sampleRate, int samplesPerBlockExpected) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    bool hasEditor() const override { return true; }
    juce::AudioProcessorEditor* createEditor() override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    static juce::StringArray getPresetNames();

    float getMeterInputPeak() const noexcept { return meterState.inputPeak.load (std::memory_order_relaxed); }
    float getMeterOutputPeak() const noexcept { return meterState.outputPeak.load (std::memory_order_relaxed); }
    float getMeterGainReductionDb() const noexcept { return meterState.gainReductionDb.load (std::memory_order_relaxed); }
    float getMeterClipWarning() const noexcept { return meterState.clipWarning.load (std::memory_order_relaxed); }

private:
    ParameterSnapshot readParameters() const;
    void resetSafetyState() noexcept;
    bool bufferContainsInvalidSamples (const juce::AudioBuffer<float>& buffer) const noexcept;

    template <typename ParamType>
    ParamType getParamValue (const juce::String& id, ParamType fallback) const
    {
        if (auto* param = apvts.getRawParameterValue (id))
            return static_cast<ParamType> (param->load());

        return fallback;
    }

    double currentSampleRate = 44100.0;
    int autoMuteSamplesRemaining = 0;
    int currentProgramIndex = 0;
    MeterState meterState;
    SignalChain signalChain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SafeToolboxAudioProcessor)
};

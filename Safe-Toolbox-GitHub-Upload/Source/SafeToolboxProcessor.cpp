#include "SafeToolboxProcessor.h"

#include "SafeToolboxEditor.h"
#include "DSP/SampleSanitizer.h"
#include "Parameters/ParameterIDs.h"
#include "Parameters/ParameterUtilities.h"
#include "Parameters/PresetDefinitions.h"

namespace
{
    template <typename T>
    T clampRange (T value, T low, T high) noexcept
    {
        return juce::jlimit (low, high, value);
    }
}

SafeToolboxAudioProcessor::SafeToolboxAudioProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "Parameters", createParameterLayout())
{
    resetSafetyState();
}

int SafeToolboxAudioProcessor::getNumPrograms()
{
    return static_cast<int> (getPresetNames().size());
}

int SafeToolboxAudioProcessor::getCurrentProgram()
{
    return currentProgramIndex;
}

void SafeToolboxAudioProcessor::setCurrentProgram (int index)
{
    index = juce::jlimit (0, juce::jmax (0, getNumPrograms() - 1), index);
    currentProgramIndex = index;
    applyPreset (*this, index);
}

const juce::String SafeToolboxAudioProcessor::getProgramName (int index)
{
    const auto names = getPresetNames();
    return juce::isPositiveAndBelow (index, names.size()) ? names[index] : juce::String();
}

void SafeToolboxAudioProcessor::changeProgramName (int, const juce::String&)
{
}

bool SafeToolboxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto input = layouts.getMainInputChannelSet();
    const auto output = layouts.getMainOutputChannelSet();
    return input == output && (input == juce::AudioChannelSet::mono() || input == juce::AudioChannelSet::stereo());
}

void SafeToolboxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlockExpected)
{
    currentSampleRate = juce::jmax (sampleRate, 1.0);
    signalChain.prepare (currentSampleRate, samplesPerBlockExpected, getTotalNumInputChannels());
    signalChain.setTransportBpm (120.0);
    resetSafetyState();
}

void SafeToolboxAudioProcessor::releaseResources()
{
    signalChain.reset();
}

juce::AudioProcessorEditor* SafeToolboxAudioProcessor::createEditor()
{
    return new SafeToolboxAudioProcessorEditor (*this);
}

void SafeToolboxAudioProcessor::resetSafetyState() noexcept
{
    autoMuteSamplesRemaining = 0;
    meterState.reset();
    signalChain.reset();
}

ParameterSnapshot SafeToolboxAudioProcessor::readParameters() const
{
    ParameterSnapshot p;

    p.inputGainDb = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::inputGain, 0.0f), -24.0f, 24.0f);
    p.safetyCeilingDb = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::safetyCeiling, -0.5f), -12.0f, -0.1f);
    p.limiterThresholdDb = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::limiterThreshold, -1.0f), p.safetyCeilingDb - 12.0f, p.safetyCeilingDb);
    p.limiterReleaseMs = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::limiterRelease, 80.0f), 5.0f, 500.0f);
    p.softClip = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::softClip, 0.0f), 0.0f, 1.0f);
    p.panicMute = getParamValue<float> (SafeToolboxParamIDs::panicMute, 0.0f) > 0.5f;

    p.driveAmount = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::driveAmount, 0.25f), 0.0f, 1.0f);
    p.driveToneHz = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::driveTone, 20000.0f), 80.0f, 20000.0f);
    p.driveMix = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::driveMix, 0.0f), 0.0f, 1.0f);
    p.driveBias = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::driveBias, 0.0f), -1.0f, 1.0f);
    p.driveOutputDb = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::driveOutput, 0.0f), -24.0f, 24.0f);
    p.driveBypass = getParamValue<float> (SafeToolboxParamIDs::driveBypass, 0.0f) > 0.5f;

    p.filterType = juce::jlimit (0, 4, static_cast<int> (std::round (getParamValue<float> (SafeToolboxParamIDs::filterType, 1.0f))));
    p.filterSlope = juce::jlimit (0, 1, static_cast<int> (std::round (getParamValue<float> (SafeToolboxParamIDs::filterSlope, 0.0f))));
    p.filterCutoffHz = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::filterCutoff, 12000.0f), 20.0f, 20000.0f);
    p.filterResonance = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::filterResonance, 0.7f), 0.2f, 12.0f);
    p.filterDrive = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::filterDrive, 0.0f), 0.0f, 1.0f);
    p.filterBypass = getParamValue<float> (SafeToolboxParamIDs::filterBypass, 0.0f) > 0.5f;

    p.delayTimeSeconds = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::delayTime, 0.18f), 0.01f, 2.0f);
    p.delaySync = juce::jlimit (0, 5, static_cast<int> (std::round (getParamValue<float> (SafeToolboxParamIDs::delaySync, 0.0f))));
    p.delayMode = juce::jlimit (0, 1, static_cast<int> (std::round (getParamValue<float> (SafeToolboxParamIDs::delayMode, 0.0f))));
    p.delayFeedback = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::delayFeedback, 0.25f), 0.0f, 0.65f);
    p.delayMix = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::delayMix, 0.25f), 0.0f, 0.5f);
    p.delayFeedbackToneHz = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::delayFeedbackTone, 6000.0f), 80.0f, 20000.0f);
    p.delayStereoOffsetMs = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::delayStereoOffset, 0.0f), 0.0f, 50.0f);
    p.delayDamping = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::delayDamping, 0.35f), 0.0f, 1.0f);
    p.delayBypass = getParamValue<float> (SafeToolboxParamIDs::delayBypass, 0.0f) > 0.5f;

    p.reverbSize = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::reverbSize, 0.35f), 0.0f, 1.0f);
    p.reverbDecay = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::reverbDecay, 0.35f), 0.0f, 1.0f);
    p.reverbDamping = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::reverbDamping, 0.35f), 0.0f, 1.0f);
    p.reverbPreDelayMs = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::reverbPreDelay, 0.0f), 0.0f, 250.0f);
    p.reverbMix = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::reverbMix, 0.18f), 0.0f, 0.5f);
    p.reverbToneHz = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::reverbTone, 9000.0f), 120.0f, 20000.0f);
    p.reverbMode = juce::jlimit (0, 2, static_cast<int> (std::round (getParamValue<float> (SafeToolboxParamIDs::reverbMode, 0.0f))));
    p.reverbWidth = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::reverbWidth, 1.0f), 0.0f, 1.0f);
    p.reverbBypass = getParamValue<float> (SafeToolboxParamIDs::reverbBypass, 0.0f) > 0.5f;

    p.wobbleRateHz = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::wobbleRate, 0.2f), 0.05f, 8.0f);
    p.wobbleDepthMs = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::wobbleDepth, 0.4f), 0.0f, 12.0f);
    p.wobbleShape = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::wobbleShape, 0.5f), 0.0f, 1.0f);
    p.wobbleStereoPhase = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::wobbleStereoPhase, 0.5f), 0.0f, 1.0f);
    p.wobbleMix = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::wobbleMix, 0.25f), 0.0f, 1.0f);
    p.wobbleMode = juce::jlimit (0, 3, static_cast<int> (std::round (getParamValue<float> (SafeToolboxParamIDs::wobbleMode, 1.0f))));
    p.wobbleBypass = getParamValue<float> (SafeToolboxParamIDs::wobbleBypass, 0.0f) > 0.5f;

    p.width = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::width, 1.0f), 0.0f, 1.5f);
    p.widthLowCutHz = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::widthLowCut, 120.0f), 20.0f, 1000.0f);
    p.widthHighCutHz = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::widthHighCut, 12000.0f), 1000.0f, 20000.0f);
    p.widthMonoBelow = getParamValue<float> (SafeToolboxParamIDs::widthMonoBelow, 0.0f) > 0.5f;
    p.balance = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::balance, 0.0f), -1.0f, 1.0f);
    p.widthBypass = getParamValue<float> (SafeToolboxParamIDs::widthBypass, 0.0f) > 0.5f;

    p.outputGainDb = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::outputGain, 0.0f), -24.0f, 24.0f);
    p.globalMix = SafeToolboxParameterUtilities::clampFloat (getParamValue<float> (SafeToolboxParamIDs::globalMix, 1.0f), 0.0f, 1.0f);
    p.bypass = getParamValue<float> (SafeToolboxParamIDs::bypass, 0.0f) > 0.5f;

    return p;
}

void SafeToolboxAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const auto numInputChannels = getTotalNumInputChannels();
    const auto numOutputChannels = getTotalNumOutputChannels();
    const auto numChannels = juce::jmin (numInputChannels, numOutputChannels);
    const auto numSamples = buffer.getNumSamples();

    if (auto* playHead = getPlayHead())
        if (const auto positionInfo = playHead->getPosition())
            if (const auto bpm = positionInfo->getBpm())
                if (*bpm > 0.0)
                    signalChain.setTransportBpm (*bpm);

    if (numChannels <= 0 || numSamples <= 0)
    {
        buffer.clear();
        return;
    }

    if (autoMuteSamplesRemaining > 0)
    {
        buffer.clear();
        autoMuteSamplesRemaining = juce::jmax (0, autoMuteSamplesRemaining - numSamples);
        meterState.clipWarning.store (1.0f, std::memory_order_relaxed);
        return;
    }

    const auto params = readParameters();

    if (params.panicMute)
    {
        buffer.clear();
        meterState.reset();
        meterState.clipWarning.store (1.0f, std::memory_order_relaxed);
        return;
    }

    meterState.reset();

    if (! signalChain.process (buffer, params, meterState) || bufferContainsInvalidSamples (buffer))
    {
        buffer.clear();
        autoMuteSamplesRemaining = static_cast<int> (currentSampleRate * 0.25);
        meterState.clipWarning.store (1.0f, std::memory_order_relaxed);
        return;
    }

    for (int ch = numChannels; ch < numOutputChannels; ++ch)
        buffer.clear (ch, 0, numSamples);
}

void SafeToolboxAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void SafeToolboxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
    {
        auto xmlText = xml->toString();
        xmlText = xmlText.replace ("reverbPredelay", "reverbPreDelay");

        if (auto migratedXml = juce::parseXML (xmlText))
        {
            if (migratedXml->hasTagName (apvts.state.getType()))
                apvts.replaceState (juce::ValueTree::fromXml (*migratedXml));
        }
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout SafeToolboxAudioProcessor::createParameterLayout()
{
    return ::createParameterLayout();
}

juce::StringArray SafeToolboxAudioProcessor::getPresetNames()
{
    return ::getPresetNames();
}

bool SafeToolboxAudioProcessor::bufferContainsInvalidSamples (const juce::AudioBuffer<float>& buffer) const noexcept
{
    return SafeToolboxSampleSanitizer::bufferContainsInvalidSamples (buffer);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SafeToolboxAudioProcessor();
}

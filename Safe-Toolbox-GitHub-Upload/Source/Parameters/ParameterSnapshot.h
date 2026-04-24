#pragma once

struct ParameterSnapshot
{
    float inputGainDb = 0.0f;
    float safetyCeilingDb = -0.5f;
    float limiterThresholdDb = -1.0f;
    float limiterReleaseMs = 80.0f;
    float softClip = 0.0f;
    bool panicMute = false;

    float driveAmount = 0.25f;
    float driveToneHz = 20000.0f;
    float driveMix = 0.0f;
    float driveBias = 0.0f;
    float driveOutputDb = 0.0f;
    bool driveBypass = false;

    int filterType = 1;
    int filterSlope = 0;
    float filterCutoffHz = 12000.0f;
    float filterResonance = 0.7f;
    float filterDrive = 0.0f;
    bool filterBypass = false;

    float delayTimeSeconds = 0.18f;
    int delaySync = 0;
    int delayMode = 0;
    float delayFeedback = 0.25f;
    float delayMix = 0.25f;
    float delayFeedbackToneHz = 6000.0f;
    float delayStereoOffsetMs = 0.0f;
    float delayDamping = 0.35f;
    bool delayBypass = false;

    float reverbSize = 0.35f;
    float reverbDecay = 0.35f;
    float reverbDamping = 0.35f;
    float reverbPreDelayMs = 0.0f;
    float reverbMix = 0.18f;
    float reverbToneHz = 9000.0f;
    int reverbMode = 0;
    float reverbWidth = 1.0f;
    bool reverbBypass = false;

    float wobbleRateHz = 0.2f;
    float wobbleDepthMs = 0.4f;
    float wobbleShape = 0.5f;
    float wobbleStereoPhase = 0.5f;
    float wobbleMix = 0.25f;
    int wobbleMode = 1;
    bool wobbleBypass = false;

    float width = 1.0f;
    float widthLowCutHz = 120.0f;
    float widthHighCutHz = 12000.0f;
    bool widthMonoBelow = false;
    float balance = 0.0f;
    bool widthBypass = false;

    float outputGainDb = 0.0f;
    float globalMix = 1.0f;
    bool bypass = false;
};

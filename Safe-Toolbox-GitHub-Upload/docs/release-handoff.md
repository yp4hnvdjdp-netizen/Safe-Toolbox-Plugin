# Safe Toolbox Release Handoff

## Status
- State: Frozen release reference
- Owner: Human operator
- Reviewers: Codex and Claude
- Last Updated: YYYY-MM-DD

## Release Summary
Safe Toolbox is a headphone-safe creative effects instrument for Logic Pro AU and standalone use.

## Verified Build / Install
- Latest build artifact:
  - `build-make/SafeToolbox_artefacts/Debug/AU/Safe Toolbox.component/Contents/MacOS/Safe Toolbox`
- Installed AU locations:
  - `/Library/Audio/Plug-Ins/Components/Safe Toolbox.component`
  - `/Users/josephshively/Library/Audio/Plug-Ins/Components/Safe Toolbox.component`

## Frozen Checklist

### Host Load
- AU loads in Logic Pro.
- Installed component matches the latest build artifact.
- Plug-in opens without a layout or attachment failure.

### Visual Integrity
- Module panels render correctly.
- Control labels are readable.
- No overlap remains in the lower utility rows.
- Hero knobs and support knobs remain visually distinct.
- Safe Mode and meters feel integrated into the shell.

### Audio Integrity
- Delay, Reverb, and Wobble do not produce the earlier crackle behavior.
- Safety limiter remains active.
- No NaN / Inf or runaway spikes.
- Playback is stable under a real imported region.

### State Integrity
- Presets still recall.
- Bypass and panic controls remain functional.
- Host session state can be saved and reopened.

### Release Integrity
- AU bundle is copied to both installed locations.
- Logic has been restarted or rescanned.
- Installed AU is the current binary.
- Release build is the only canonical build from this point.

## Frozen Product Map

### Identity
- Safe Toolbox is a headphone-safe creative effects instrument.
- Safety is the identity, not a background feature.

### Interface Shell
- Header
- Preset bar
- Module row
- Safe Mode area
- Meter strip

### Modules
- Input: staging and trim
- Drive: saturation and tone
- Filter: shaping and mode control
- Delay: space and rhythm
- Reverb: ambience and depth
- Wobble: modulation and instability
- Width: stereo image
- Output: final gain and protection

### Safety System
- DC blocker
- NaN / Inf guard
- limiter ceiling
- panic mute
- conservative time-based processing
- denormal prevention

### UI System
- reusable module cards
- stronger border hierarchy
- hero vs support knob language
- clearer button and combo box spacing
- finished Safe Mode and meter presentation

### Host System
- Logic Pro AU validation
- automation
- preset recall
- save / reopen
- playback stability
- offline bounce readiness

### Release System
- final UI polish only if truly necessary
- no more structural redesign
- no more parameter renames
- no more host-compatibility churn

## Notes
- This file is the canonical freeze reference.
- If a new regression is confirmed, document it in `known-issues.md` and reopen the roadmap.

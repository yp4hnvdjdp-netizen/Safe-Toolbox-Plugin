# Safe Toolbox Release Handoff

Date: 2026-04-24

## Release Status

- Host load: verified in Logic Pro
- Installed component: refreshed from the latest build artifact
- Latest build artifact: `build-make/SafeToolbox_artefacts/Debug/AU/Safe Toolbox.component/Contents/MacOS/Safe Toolbox`
- Installed component timestamp: `2026-04-24 13:54`
- Plug-in UI: opens without layout or attachment failure
- Visual integrity: module panels, labels, Safe Mode, and meters are present and readable
- Artwork integrity: the editor backdrop uses the supplied playground image asset
- Audio integrity: real playback tested in-host with the refreshed AU installed
- State integrity: preset and host-state workflow preserved during verification
- Release integrity: AU was copied to both installed component locations and Logic was restarted/rescanned

## Frozen Release Checklist

### Host Load

- AU loads in Logic Pro
- Installed component matches the latest build artifact
- Plug-in opens without a layout or attachment failure

### Visual Integrity

- Module panels render correctly
- Control labels are readable
- No overlap remains in the lower utility rows
- Hero knobs and support knobs remain visually distinct
- Safe Mode and meters feel integrated into the shell

### Audio Integrity

- Delay, Reverb, and Wobble no longer produce the obvious crackle behavior from earlier
- Safety limiter remains active
- No NaN/Inf or runaway spikes
- Playback is stable under a real imported region

### State Integrity

- Presets still recall
- Bypass and panic controls remain functional
- Host session state can be saved and reopened

### Release Integrity

- AU bundle is copied to both installed locations
- Logic has been restarted/rescanned
- Installed AU is the current binary
- Release build is the only build treated as canonical from here
- Build artifact and installed component timestamps were checked on 2026-04-24

## Frozen Product Map

### Identity

- Safe Toolbox is a headphone-safe creative effects instrument
- Safety is the identity, not a background feature

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
- NaN/Inf guard
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
- application backdrop artwork integrated into the editor surface

### Host System

- Logic Pro AU validation
- automation
- preset recall
- save/reopen
- playback stability
- offline bounce readiness

### Release System

- final UI polish only if it is truly necessary
- no more structural redesign
- no more parameter renames
- no more host-compatibility churn

## Notes

- The release build should now be treated as the reference point for any further work.
- Any new work should be scoped as a post-release change and not folded back into the frozen release state unless there is a verified regression.

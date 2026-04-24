# Safe Toolbox Roadmap

## Purpose
Define the current direction and sequencing for the plugin.

## Current Phase
- Phase: Final stabilization and handoff
- Priority: Host reliability, visual integrity, audio integrity
- Owner: Claude for roadmap order, Codex for implementation

## Near-Term Goals
1. Restore and verify AU control visibility in Logic Pro.
2. Eliminate the remaining crackle in time-based DSP.
3. Tighten module spacing, labels, and knob proportion consistency.
4. Keep Safe Mode and limiter behavior intact.
5. Preserve release documentation and archive readiness.

## UI Roadmap
- Stabilize editor bounds and host scaling behavior.
- Keep hero controls visually dominant and support controls lighter.
- Clean up lower-row labels and combo boxes.
- Ensure meters and Safe Mode feel integrated into the shell.
- Keep the backdrop artwork visually balanced in AU hosts.

## DSP Roadmap
- Verify Delay, Reverb, and Wobble for crackle sources.
- Confirm buffer wrap logic and interpolation safety.
- Check denormal protection and smoothing on automation.
- Preserve the safety limiter and panic mute behavior.

## Review Roadmap
- Every implementation step should end in a diff review.
- Claude reviews architecture and risk.
- Codex applies the smallest safe fix.
- Human validates with build output, screenshots, and host playback.

## Release Criteria
- AU loads in Logic Pro without stale cache confusion.
- Controls are fully visible and readable.
- Audio is stable under playback.
- Safe Mode remains protective and transparent.
- Release docs and archive docs are complete.

## Change Log
| Date | Change | Reason |
|---|---|---|
| YYYY-MM-DD | Initial roadmap entry | Establish collaboration sequence |

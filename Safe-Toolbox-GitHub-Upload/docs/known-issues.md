# Known Issues

## Purpose
Track confirmed bugs, layout defects, audio artifacts, and host-specific risks.

## Issue Status Key
- Open
- Investigating
- Fixed Pending Verification
- Verified Fixed
- Deferred

## Current Issues

| ID | Status | Severity | Area | Description | Evidence | Owner | Next Step |
|---|---|---|---|---|---|---|---|
| KI-001 | Verified Fixed | High | Logic AU UI | Controls appear reduced or hidden in AU compared with standalone. | `debug/logic-au-current.png`, Logic host verification, standalone comparison | Codex | Closed; next cycle is KI-002 crackle review. |
| KI-002 | Investigating | High | DSP | Crackling heard during playback in time-based effects. | Audio test / Logic playback | Codex | Inspect Delay, Reverb, Wobble smoothing and wrap logic. |
| KI-003 | Open | Medium | UI | Lower-row labels and combo boxes may overlap in narrow panels. | Screenshot | Codex | Tighten layout and reduce text density. |
| KI-004 | Open | Medium | UI | Knobs may not use the full visible perimeter. | Screenshot | Codex | Adjust rotary rendering and proportions. |
| KI-005 | Open | Medium | Release | AU cache may still load an older installed binary. | Install timestamp / host restart notes | Human | Restart Logic or rescan AU. |

## Deferred Issues
List anything intentionally postponed.

| ID | Reason Deferred | Revisit Trigger |
|---|---|---|
| DI-001 | Example: Cosmetic polish beyond release freeze | After host stability is confirmed |

## Verified Fixes
List confirmed resolved items.

| ID | Fixed In | Verification |
|---|---|---|
| KI-001 | 2026-04-24 | Safe Toolbox AU editor is visible in Logic Pro with all module controls, Safe Mode, and meters present; screenshots captured for both hosts. |
| VF-001 | YYYY-MM-DD | Build succeeded and Logic verified |
| VF-002 | YYYY-MM-DD | Standalone and AU matched |

## Notes
- Keep this file strictly factual.
- If an issue is not reproducible anymore, move it out of Current Issues and into Verified Fixes or Deferred.
- Do not use this file for general brainstorming.

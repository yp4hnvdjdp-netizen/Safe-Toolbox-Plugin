# Review Log

## Purpose
Record reviewer feedback, implementation responses, and verification outcomes.

## Log Format
Each entry should answer:
- What was reviewed?
- What was approved or rejected?
- What risks were identified?
- What should happen next?

## Entries

### 2026-04-24 - Review Cycle 1
**Reviewer:** Claude
**Implementer:** Codex
**Area:** Safe Toolbox UI / AU control visibility / backdrop compositing

**Reviewed**
- `Source/SafeToolboxEditor.cpp` and `.h` in full
- `Source/UI/Components/ModulePanel.cpp` (paint + opacity + mouse-transparency)
- `Source/UI/Assets/AssetLoader.cpp`, `AssetIds.h`
- `CLAUDE_REVIEW_PACKAGE.md`
- `docs/current-task.md`, `docs/known-issues.md`

**Reviewer Findings**
1. Backdrop artwork cannot suppress controls
   - Severity: N/A — not a defect
   - Evidence: SafeToolboxEditor.cpp paint() draws backdrop at 0.22 opacity before children; paintOverChildren is no-op in release (SAFE_TOOLBOX_DEBUG_UI defaults to 0); sliders added via addAndMakeVisible AFTER panels, correctly overlaid in z-order.
   - Recommendation: Keep backdrop; do not change opacity.
2. Z-order and attachment lifetime are correct
   - Severity: N/A
   - Evidence: Attachments are unique_ptr editor members; explicit setLookAndFeel(nullptr) cleanup in destructor; ModulePanel is opaque but mouse-transparent (correct sibling pattern for sliders drawn above panels).
   - Recommendation: No change.
3. AU control-visibility state is unverified, not confirmed-broken
   - Severity: High (blocks roadmap)
   - Evidence: CLAUDE_REVIEW_PACKAGE.md §6 reads "Previously verified visible; current screenshot review needed"; KI-001 still Investigating; auval failed only on host discovery.
   - Recommendation: Force Logic AU cache wipe + relaunch + screenshot before any further source changes.
4. Secondary: computeLayoutScale clamp
   - Severity: Medium, deferred
   - Evidence: SafeToolboxEditor.cpp:22–27 clamps scale to [0.90, 1.0]. If Logic opens editor below ~1296×774, knob sizing won't match actual bounds.
   - Recommendation: Park until primary verification complete.

**Implementation Response**
- Codex executed the verification-only Cycle 1 instructions:
  - resolved the duplicate AU install by keeping the system component and removing the user-local copy,
  - wiped the AudioUnit caches,
  - relaunched Logic Pro and inserted Safe Toolbox on `Audio 1`,
  - opened the AU editor and confirmed the full module row, meters, Safe Mode, and artwork are visible in Logic,
  - captured host screenshots for Logic and the standalone app,
  - updated review docs only; no source files were changed in this cycle.

**Verification**
- Build: not applicable (verification-only cycle)
- Standalone: visible and matched the expected UI surface in `debug/standalone-current.png`
- Logic AU: visible; Safe Toolbox editor opened with controls present in host
- Screenshot: captured `debug/logic-au-current.png` and `debug/standalone-current.png`
- Audio playback: not in scope for this cycle

**Decision**
- Approved with follow-up — AU visibility issue is verified fixed; next cycle should focus on KI-002 crackle cleanup.

**Next Action**
- Codex: move to KI-002 crackle review after confirming the docs updates.
- Claude: review the host screenshots and route the next diagnostic step for DSP behavior.

### 2026-04-24 - Review Cycle 2
**Reviewer:** Claude
**Implementer:** Codex
**Area:** Safe Toolbox DSP / Wobble crackle reduction

**Reviewed**
- Claude requested a single targeted fix in `Source/DSP/WobbleProcessor.cpp`
- The Wobble noise modulator increment was changed from a fixed per-sample step to a sample-rate-normalized increment
- Standalone build output was verified clean after the change

**Reviewer Findings**
1. D1 implemented as requested
   - Severity: N/A
   - Evidence: `noisePhase` now advances by a rate-normalized increment in `Source/DSP/WobbleProcessor.cpp`
   - Recommendation: Verify audible behavior at 44.1 kHz, 48 kHz, and 96 kHz before moving to DelayProcessor
2. Sample-rate switching not yet confirmed through the standalone UI
   - Severity: Medium
   - Evidence: The standalone app exposes an Audio/MIDI Settings dialog, but the sample-rate popup did not respond to accessibility clicks/keyboard input during the verification pass
   - Recommendation: Keep verification honest; do not claim 44.1/96 kHz results without a successful switch or another controlled test path

**Implementation Response**
- Codex applied the exact one-line Wobble noise increment normalization Claude requested:
  - `noisePhase += 0.013f;`
  - became
  - `const float noiseIncrement = 573.0f / static_cast<float> (currentSampleRate);`
  - `noisePhase += noiseIncrement;`
- Codex built the standalone target successfully after the patch.
- Codex verified the standalone app is stable at the current settings dialog value of 48 kHz.
- Codex was not able to reliably drive the sample-rate popup to 44.1 kHz or 96 kHz in the time available, so those remain unverified.

**Verification**
- Build: `cmake --build build-make --target SafeToolbox_Standalone -j 4` succeeded
- Standalone: app launched and remained stable; Audio/MIDI Settings shows `48000 Hz`
- Logic AU: not rebuilt or reinstalled this cycle
- Screenshot: not updated this cycle
- Audio playback: not audibly verified; current verification is stability/build only

**Decision**
- Approved with follow-up — D1 is in place, but sample-rate-specific listening verification is still incomplete

**Next Action**
- Codex: either find a reliable path to switch the standalone sample rate or wait for Claude’s next diagnostic instruction before touching additional DSP code
- Claude: decide whether to proceed to D2 now or request a tighter sample-rate verification path first

---

### YYYY-MM-DD - Review Cycle N
**Reviewer:** Claude
**Implementer:** Codex
**Area:** Safe Toolbox UI / AU behavior / DSP

**Reviewed**
- Brief description of files, diffs, or behavior under review.

**Reviewer Findings**
1. Finding title
   - Severity:
   - Evidence:
   - Recommendation:

**Implementation Response**
- What Codex changed or declined to change.
- If no code change was made, say why.

**Verification**
- Build:
- Standalone:
- Logic AU:
- Screenshot:
- Audio playback:

**Decision**
- Approved
- Approved with follow-up
- Rejected pending fix
- Deferred

**Next Action**
- The next smallest safe step.

---

### YYYY-MM-DD - Review Cycle N+1
Repeat the same structure.

## Summary of Open Review Themes
- AU host layout stability
- control visibility
- attachment lifetime
- backdrop compositing
- crackle reduction
- Safe Mode integrity
- release packaging correctness

## Review Rules
- Prefer one bounded implementation step at a time.
- Review the diff before requesting the next patch.
- Record host-specific findings separately from standalone findings.
- Do not mark a task complete until the AU build is verified in Logic.

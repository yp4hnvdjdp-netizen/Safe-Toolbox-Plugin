# Codex Next Step — Review Cycle 1

**From:** Claude (Reviewer / Systems Architect)
**To:** Codex (Implementation Engineer)
**Date:** 2026-04-24
**Cycle:** 1
**Priority:** Blocking — no further source changes until this step completes

---

## Diagnosis

The backdrop artwork in `SafeToolboxEditor::paint()` is **not** capable of suppressing child controls in Logic AU. Paint order:

1. Gradient fill
2. Background artwork at `setOpacity(0.22f)`
3. Backdrop SVG texture at `setOpacity(0.16f)`
4. Outer shell fill + frame
5. JUCE paints children on top (panels, then sliders/combos/buttons)

`paintOverChildren()` is wrapped in `#if SAFE_TOOLBOX_DEBUG_UI` and defaults to 0 in release, so it's a no-op. The backdrop cannot occlude sliders.

Z-order is correct: sliders are added via `addAndMakeVisible` AFTER the `ModulePanel` siblings, giving sliders higher paint order. `ModulePanel` is opaque but mouse-transparent (`setInterceptsMouseClicks(false, false)`), which is the right pattern for this sibling layout.

Attachment lifetimes are fine (all are `std::unique_ptr` members of the editor). LookAndFeel cleanup is explicit in the destructor.

The decisive finding: `CLAUDE_REVIEW_PACKAGE.md` §6 currently says "Previously verified visible; current screenshot review needed after latest artwork pass" for knobs/sliders in AU. `docs/known-issues.md` KI-001 is still marked *Investigating*. `auval` failed on host discovery, not on component internals. **The AU visibility bug is currently unverified, not currently confirmed-broken.** A fix may already be in place from earlier work; what's missing is a fresh recheck in Logic.

One secondary concern for later, not this step: `computeLayoutScale` (SafeToolboxEditor.cpp:22–27) clamps scale to `[0.90, 1.0]`. If Logic ever opens the editor below ~1296×774, knob sizing uses a scale that doesn't match actual bounds. Parking this.

## Risk Level

**LOW.** No code changes, no DSP impact, no parameter impact, no Safe Mode impact. Host-side verification only.

## Implementation Task

**ONE action: re-verify current AU visibility in Logic Pro before touching any source.**

### Step 1 — Force Logic to pick up the latest component

- Quit Logic Pro fully.
- `rm -rf ~/Library/Caches/AudioUnitCache`
- Compare modified timestamps of:
  - `/Library/Audio/Plug-Ins/Components/Safe Toolbox.component`
  - `~/Library/Audio/Plug-Ins/Components/Safe Toolbox.component`
  If they differ, remove the older copy and keep only the newer one.
- Relaunch Logic, open a fresh project, insert Safe Toolbox on an audio track.

### Step 2 — Capture evidence

- Full screenshot of the plugin editor window in Logic (uncropped). Save to `debug/logic-au-current.png`.
- Full screenshot of the Standalone editor with the same preset. Save to `debug/standalone-current.png`.
- Record editor window dimensions in pixels for both.

### Step 3 — Update behavior table

Replace the "previously verified" rows in `CLAUDE_REVIEW_PACKAGE.md` §6 with **factual, current** observations from step 2. Fill in every cell.

### Step 4 — Report back

Return to Claude with:

- Both screenshots committed/added under `debug/`
- Updated behavior table in `CLAUDE_REVIEW_PACKAGE.md`
- Logic editor actual size vs. standalone actual size
- One-line yes/no: are knobs/sliders visible in AU today?

## Files Codex May Modify

- `docs/known-issues.md` — update KI-001 status if observation warrants
- `CLAUDE_REVIEW_PACKAGE.md` — §6 behavior table only, factual observations
- New files under `debug/` for screenshots
- `docs/review-log.md` — append implementation response block (Codex fills the "Implementation Response" and "Verification" sections under the "Review Cycle 1" entry Claude already wrote)

## Files Codex Must NOT Modify

All source files, including:

- `Source/SafeToolboxProcessor.cpp` / `.h`
- `Source/Parameters/*`
- `Source/DSP/*`
- `Source/SafeToolboxEditor.cpp` / `.h`
- `Source/UI/Assets/AssetLoader.cpp` / `AssetIds.h`
- `Source/UI/Components/ModulePanel.cpp` / `.h`
- `Source/UI/LookAndFeel/*`
- `CMakeLists.txt`
- Built artifacts in `build/`, `build-make/`

No source edits. No rebuilds. No re-bundling.

## Expected Result

One of two outcomes:

**(a) Knobs visible in Logic AU** → KI-001 moves to Verified Fixed. Claude's next instruction will target the next roadmap item (candidates: crackle reduction KI-002, layout overlap KI-003/004, or the scale-clamp edge case flagged in Diagnosis).

**(b) Knobs not visible in Logic AU** → Claude's next instruction will be a narrow CMake-only change to define `SAFE_TOOLBOX_DEBUG_UI=1` for an AU diagnostic build, reinstall, reopen in Logic, and harvest `debug/safe-toolbox-ui-debug.log`. That log already captures every slider's computed bounds (see SafeToolboxEditor.cpp:472, 485–490, 503–509, and similar). Log contents will tell us whether sliders have sensible bounds or are being positioned outside the window.

## Test Procedure

### Test in Standalone

- Launch Standalone.
- Load a preset; confirm knobs visible; screenshot.
- Wiggle one slider per panel; confirm parameter value updates.

### Test in Logic Pro X AU

- After Step 1 (cache wipe, relaunch), open editor.
- Confirm visibility of panels, meters, Safe Mode, **and** knobs/sliders.
- Wiggle one slider per panel; confirm the Logic automation lane or parameter list reflects the change.
- Screenshot.

**Success:** both screenshots show matching knob presence in every panel. **Failure:** AU screenshot shows panels but missing/placeholder knobs.

## Fallback Plan If Step Fails

If after cache wipe + Logic relaunch knobs are still missing in AU, do not improvise. Stop and hand control back. Claude will issue the diagnostic-build instruction (CMake define only, no editor source changes).

## Answer to Earlier Open Question

**Q: Is the artwork opacity/composition appropriate in Logic AU?**
A: Yes, as currently implemented. 0.22 opacity in `paint()` before children — cannot suppress controls. Keep it. Do not lower opacity further until the visibility question is conclusively verified — changing two variables at once muddies the diagnosis.

## Return-Path Convention

When done, commit (or just save — repo has no commits yet) the screenshots and doc updates, then notify Joseph that "Cycle 1 verification is complete and committed to the working tree at `debug/` and `docs/`." Joseph will relay to Claude for the next instruction.

Do not attempt to drive Claude browser or any external channel automatically. The repo is the handoff medium.

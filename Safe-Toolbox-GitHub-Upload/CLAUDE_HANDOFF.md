# Safe Toolbox Claude Handoff

This file is the single entry point for Claude’s review pass. Read this first, then inspect the attached files in the order below.

## Current State

- Project: Safe Toolbox JUCE audio plugin
- Phase: final stabilization and handoff
- Current focus:
  - AU / Logic Pro UI stability
  - control visibility in Logic AU vs standalone
  - layout correctness and spacing
  - crackle reduction in time-based DSP
  - Safe Mode integrity and release freeze

## What Was Recently Changed

- Added the supplied artwork as the editor backdrop:
  - [Assets/colorbox_ui.png](./Assets/colorbox_ui.png)
- Wired the artwork into the editor via the binary asset pipeline:
  - [Source/UI/Assets/AssetIds.h](./Source/UI/Assets/AssetIds.h)
  - [Source/SafeToolboxEditor.h](./Source/SafeToolboxEditor.h)
  - [Source/SafeToolboxEditor.cpp](./Source/SafeToolboxEditor.cpp)
- Kept the existing vector backdrop texture overlay, but reduced its opacity so the new artwork is the dominant background layer.
- Added and documented the collaboration pack under [docs/](./docs).

## What Claude Should Review

1. Confirm whether control visibility issues in Logic AU are still plausibly layout/compositing related.
2. Verify that the backdrop artwork path is safe and does not suppress children in AU hosts.
3. Check whether the editor constructor, attachments, and `resized()` logic remain correct.
4. Review the release freeze documents and confirm the package is ready for the next diagnostic step.

## Review Order

### 1) Read these live task files first
- [docs/current-task.md](./docs/current-task.md)
- [docs/known-issues.md](./docs/known-issues.md)
- [docs/review-log.md](./docs/review-log.md)
- [docs/roadmap.md](./docs/roadmap.md)
- [docs/standup-notes.md](./docs/standup-notes.md)

### 2) Then read the frozen release reference
- [docs/release-handoff.md](./docs/release-handoff.md)
- [RELEASE_HANDOFF.md](./RELEASE_HANDOFF.md)
- [ARCHIVE_GUIDE.md](./ARCHIVE_GUIDE.md)

### 3) Then inspect the implementation files
- [Source/SafeToolboxEditor.cpp](./Source/SafeToolboxEditor.cpp)
- [Source/SafeToolboxEditor.h](./Source/SafeToolboxEditor.h)
- [Source/UI/Assets/AssetLoader.cpp](./Source/UI/Assets/AssetLoader.cpp)
- [Source/UI/Assets/AssetIds.h](./Source/UI/Assets/AssetIds.h)
- [Source/UI/Components/ModulePanel.cpp](./Source/UI/Components/ModulePanel.cpp)
- [Source/UI/LookAndFeel/SafeToolboxLookAndFeel.cpp](./Source/UI/LookAndFeel/SafeToolboxLookAndFeel.cpp)
- [Source/UI/LookAndFeel/RotaryKnobLookAndFeel.cpp](./Source/UI/LookAndFeel/RotaryKnobLookAndFeel.cpp)
- [Source/Parameters/ParameterLayout.cpp](./Source/Parameters/ParameterLayout.cpp)

## Key Evidence to Check

- Build artifact:
  - `build-make/SafeToolbox_artefacts/Debug/AU/Safe Toolbox.component/Contents/MacOS/Safe Toolbox`
- Installed AU locations:
  - `/Library/Audio/Plug-Ins/Components/Safe Toolbox.component`
  - `~/Library/Audio/Plug-Ins/Components/Safe Toolbox.component`
- BinaryData asset presence:
  - `Assets/colorbox_ui.png`
  - generated binary data output in `build-make/juce_binarydata_SafeToolboxAssets/JuceLibraryCode/BinaryData.h`

## Collaboration Rules

- Claude is the architecture/review authority unless the human operator overrides scope.
- Codex should make the smallest safe implementation step requested by review.
- No parameter renames.
- No Safe Mode protection removal.
- No broad DSP refactors unless explicitly requested.
- Treat `docs/release-handoff.md` as the freeze reference.

## If You Need the Full Package

Attachments in this repo:
- [CLAUDE_REVIEW_PACKAGE.md](./CLAUDE_REVIEW_PACKAGE.md)
- [docs/](./docs)
- [RELEASE_HANDOFF.md](./RELEASE_HANDOFF.md)
- [ARCHIVE_GUIDE.md](./ARCHIVE_GUIDE.md)
- [README.md](./README.md)
- [scripts/install_au.sh](./scripts/install_au.sh)

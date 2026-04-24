# Safe Toolbox Claude Review Package

This file is the compact package to hand to Claude for review.

## 1) Source files to inspect verbatim

Read these files directly in the repo. They contain the exact implementation, not summaries.

- `Source/SafeToolboxEditor.cpp`
- `Source/SafeToolboxEditor.h`
- `Source/UI/Assets/AssetLoader.cpp`
- `Source/UI/Assets/AssetIds.h`
- `Source/SafeToolboxProcessor.cpp`
- `Source/SafeToolboxProcessor.h`
- `Source/Parameters/ParameterLayout.cpp`
- `Source/Parameters/ParameterLayout.h`
- `Source/UI/Components/ModulePanel.cpp`
- `Source/UI/Components/ModulePanel.h`
- `Source/UI/LookAndFeel/SafeToolboxLookAndFeel.cpp`
- `Source/UI/LookAndFeel/RotaryKnobLookAndFeel.cpp`

### Notes for Claude

- There is no separate `DrivePanel.cpp` / `DrivePanel.h` file in this repo.
- The closest representative panel implementation is `ModulePanel`, which is instantiated for Input, Drive, Filter, Delay, Reverb, Wobble, Width, and Output.
- All module controls are constructed and attached in `Source/SafeToolboxEditor.cpp`.

## 2) Processor / APVTS facts

- Processor class:
  - `Source/SafeToolboxProcessor.cpp`
  - `Source/SafeToolboxProcessor.h`
- `createEditor()` is in `Source/SafeToolboxProcessor.cpp`
- APVTS member declaration is in `Source/SafeToolboxProcessor.h`
- Parameter layout source:
  - `Source/Parameters/ParameterLayout.cpp`
  - `Source/Parameters/ParameterLayout.h`

### Full APVTS layout entry point
- Function body: `createParameterLayout()` in `Source/Parameters/ParameterLayout.cpp`

## 3) Build system facts

- Build system: CMake
- Relevant snippet from `CMakeLists.txt`:

```cmake
set(SAFE_TOOLBOX_BACKGROUND_IMAGE "${CMAKE_CURRENT_SOURCE_DIR}/Assets/colorbox_ui.png")
if (EXISTS "${SAFE_TOOLBOX_BACKGROUND_IMAGE}")
    juce_add_binary_data(SafeToolboxAssets
        SOURCES
            "${SAFE_TOOLBOX_BACKGROUND_IMAGE}"
            ${SAFE_TOOLBOX_EXPORT_SVGS})
endif()

if (TARGET SafeToolboxAssets)
    target_link_libraries(SafeToolbox PRIVATE SafeToolboxAssets)
endif()
```

- This means `colorbox_ui.png` is compiled into the shared `SafeToolboxAssets` binary-data target, and that target is linked into both AU and Standalone builds.

### Generated BinaryData header top

The top of `build-make/juce_binarydata_SafeToolboxAssets/JuceLibraryCode/BinaryData.h` includes:

```cpp
namespace BinaryData
{
    extern const char*   colorbox_ui_png;
    const int            colorbox_ui_pngSize = 839657;

    extern const char*   safe_toolbox_brand_mark_svg;
    const int            safe_toolbox_brand_mark_svgSize = 650;
    ...
    const int namedResourceListSize = 15;
}
```

## 4) Runtime facts

- macOS:
  - `ProductVersion: 15.5`
  - `BuildVersion: 24F74`
- Xcode:
  - `Xcode 16.4`
  - `Build version 16F6`
- Logic Pro:
  - `CFBundleShortVersionString: 11.2.2`
  - `CFBundleVersion: 6387`
  - `CFBundleIdentifier: com.apple.logic10`
- Installed AU bundle locations:
  - `/Library/Audio/Plug-Ins/Components/Safe Toolbox.component`
  - `/Users/josephshively/Library/Audio/Plug-Ins/Components/Safe Toolbox.component`

### auval output

Command used:

```bash
auval -v aufx Stbx Safe
```

Observed output:

```text
AU Validation Tool
Version: 1.10.0
Copyright 2003-2019, Apple Inc. All Rights Reserved.
Specify -h (-help) for command options

--------------------------------------------------
VALIDATING AUDIO UNIT: 'aufx' - 'Stbx' - 'Safe'
--------------------------------------------------
ERROR: Cannot get Component's Name strings
ERROR: Error from retrieving Component Version: -50

* * FAIL
--------------------------------------------------
TESTING OPEN TIMES:
FATAL ERROR: didn't find the component
```

Interpretation:
- The AU validation tool did not resolve the component by code/name in this sandboxed run.
- This is a host discovery problem, not proof that the source bundle is broken.

### Console / log evidence

- A direct `log show` query could not be run from the sandbox.
- Therefore, Console.app output is not attached in this package.
- If Claude needs console evidence, the human should export it separately from Console.app or run an unsandboxed log capture.

## 5) Repository state

- Git repository exists at this path.
- Current branch:
  - `main`
- Status:
  - no commits yet on `main`
  - working tree is currently entirely uncommitted
- `git status --short --branch` currently reports:

```text
## No commits yet on main
?? .gitignore
?? ARCHIVE_GUIDE.md
?? Assets/
?? CLAUDE_HANDOFF.md
?? CLAUDE_REVIEW_PACKAGE.md
?? CMakeLists.txt
?? README.md
?? RELEASE_HANDOFF.md
?? Source/
?? Tests/
?? docs/
?? scripts/
```

- `git rev-parse HEAD` is unavailable because the repository has no commits yet.

## 6) Behavior table for Claude

| Control group | Standalone | Logic AU | Notes |
|---|---|---|---|
| Panel backgrounds | Yes | Yes | Drawn in `ModulePanel::paint()` |
| Knobs / sliders inside panels | Yes | Yes | Added via `addAndMakeVisible()` and attached in editor constructor |
| Meters | Yes | Yes | `MeterComponent` children in editor |
| Safe Mode section | Yes | Yes | `SafeModePanel` in editor |
| New backdrop image | Yes | Yes | Drawn in `SafeToolboxAudioProcessorEditor::paint()` |

## 7) Current files that matter most for the AU visibility question

- `Source/SafeToolboxEditor.cpp`
- `Source/SafeToolboxEditor.h`
- `Source/UI/Assets/AssetLoader.cpp`
- `Source/UI/Assets/AssetIds.h`
- `Source/UI/Components/ModulePanel.cpp`
- `Source/UI/LookAndFeel/SafeToolboxLookAndFeel.cpp`
- `Source/UI/LookAndFeel/RotaryKnobLookAndFeel.cpp`
- `Source/Parameters/ParameterLayout.cpp`

## 8) What Claude should decide next

1. Whether the backdrop artwork is safe to keep as the dominant background layer in Logic AU.
2. Whether the visibility issue, if still reproducible, is more likely a Logic compositing/cache issue than a layout bug.
3. Which smallest diagnostic step should happen next:
   - constructor / `resized()` logging, or
   - temporary backdrop bypass, or
   - a targeted Logic AU recheck with the latest installed component.

# Safe Toolbox Archive Guide

This guide is the handoff-friendly packaging reference for the frozen Safe Toolbox release state.

## What To Keep

- `Source/`
- `Assets/`
- `Tests/`
- `CMakeLists.txt`
- `RELEASE_HANDOFF.md`
- `ARCHIVE_GUIDE.md`
- `.gitignore`

## What To Exclude From Archives

- `build/`
- `build-make/`
- `DerivedData/`
- `debug/`
- `.DS_Store`
- `*.log`
- `*.tmp`
- Xcode user-state files and derived metadata

## Release Build Locations

- `/Library/Audio/Plug-Ins/Components/Safe Toolbox.component`
- `~/Library/Audio/Plug-Ins/Components/Safe Toolbox.component`

## Release Packaging Flow

1. Build the project from the frozen release source.
2. Copy the resulting AU component to both installed locations with `scripts/install_au.sh`.
3. Restart or rescan Logic Pro so it reloads the refreshed binary.
4. Confirm the installed component matches the latest build artifact.
5. Treat the verified installed binary as the canonical release build.

## Archive Packaging Flow

1. Start from a clean working tree with build and debug outputs excluded.
2. Bundle the source, assets, tests, and release handoff docs.
3. Omit transient build trees, editor caches, and host-specific scratch files.
4. Preserve the installed AU component only as a reference artifact if needed.

## Notes For Hand-Off

- The product map is frozen in `RELEASE_HANDOFF.md`.
- Any future work should be treated as post-release unless a regression is verified.
- Debug logging and scratch assets are intentionally not part of the archive payload.
- `scripts/install_au.sh` is the canonical install helper for local validation and handoff prep.

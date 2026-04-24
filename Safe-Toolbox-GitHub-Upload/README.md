# Safe Toolbox

Safe Toolbox is a JUCE audio plugin focused on headphone-safe creative effects with always-on protection and a modular UI.

## Current State

- Logic Pro AU verification completed
- Release checklist frozen in `RELEASE_HANDOFF.md`
- Archive guidance in `ARCHIVE_GUIDE.md`
- Local install helper in `scripts/install_au.sh`

## Build

```bash
cmake -S . -B build-make
cmake --build build-make -j 4
```

## Install AU For Logic Pro

```bash
scripts/install_au.sh
```

This copies the built AU component into both standard install locations:

- `/Library/Audio/Plug-Ins/Components/Safe Toolbox.component`
- `~/Library/Audio/Plug-Ins/Components/Safe Toolbox.component`

## Release References

- `RELEASE_HANDOFF.md`
- `ARCHIVE_GUIDE.md`

## Notes

- Treat the frozen release state as canonical unless a verified regression requires a post-release change.
- Keep build outputs and editor/debug artifacts out of archives.

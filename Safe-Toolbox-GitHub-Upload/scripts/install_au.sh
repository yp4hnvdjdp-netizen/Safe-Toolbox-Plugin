#!/bin/zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_AU="$ROOT_DIR/build-make/SafeToolbox_artefacts/Debug/AU/Safe Toolbox.component"
SYSTEM_DEST="/Library/Audio/Plug-Ins/Components/Safe Toolbox.component"
USER_DEST="$HOME/Library/Audio/Plug-Ins/Components/Safe Toolbox.component"

if [[ ! -d "$BUILD_AU" ]]; then
  echo "Missing AU build artifact: $BUILD_AU" >&2
  exit 1
fi

mkdir -p "/Library/Audio/Plug-Ins/Components"
mkdir -p "$HOME/Library/Audio/Plug-Ins/Components"

rm -rf "$SYSTEM_DEST"
rm -rf "$USER_DEST"

cp -R "$BUILD_AU" "$SYSTEM_DEST"
cp -R "$BUILD_AU" "$USER_DEST"

echo "Installed AU to:"
echo "  $SYSTEM_DEST"
echo "  $USER_DEST"

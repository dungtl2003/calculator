#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/.."
MANIFEST="$ROOT_DIR/build/install_manifest.txt"

# Ensure the script is run with sudo
if [ "$EUID" -ne 0 ]; then
    echo "Error: This script must be run as root. Try: sudo $SCRIPT_DIR/uninstall.sh"
    exit 1
fi

# Check if the manifest exists
if [ ! -f "$MANIFEST" ]; then
    echo "Error: Cannot find $MANIFEST."
    echo "You must build and install the project before you can uninstall it."
    exit 1
fi

echo "Removing installed files..."
# Read the manifest and force remove each file
xargs rm -f < "$MANIFEST"

echo "Cleaning up empty project directories..."
# Safely attempt to remove parent directories of the installed files
# It will silently ignore directories that still contain other files (like /usr/bin)
while read -r file; do
    dir=$(dirname "$file")
    if [ -d "$dir" ]; then
        rmdir -p --ignore-fail-on-non-empty "$dir" 2>/dev/null
    fi
done < "$MANIFEST"

echo "Refreshing Ubuntu desktop caches..."
# Update the caches so the icon is removed from the app launcher
gtk-update-icon-cache -f -t /usr/share/icons/hicolor &>/dev/null
update-desktop-database /usr/share/applications &>/dev/null

echo "Uninstallation complete."

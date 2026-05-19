#!/bin/bash
# uninstall.sh
# Removes the 'matfun' command from your system.

set -eo pipefail

INSTALL_DIR="$HOME/.local/bin"
SYSTEM_INSTALL_DIR="/usr/local/bin"

echo "=== Matfun Uninstaller ==="

targets=("$SYSTEM_INSTALL_DIR/matfun" "$INSTALL_DIR/matfun")
removed=0

for target in "${targets[@]}"; do
    if [ -f "$target" ]; then
        rm "$target"
        echo "  Removed $target"
        removed=1
    fi
done

if [ $removed -eq 1 ]; then
    echo ""
    echo "  Uninstall complete."
else
    echo "  Matfun not found - nothing to remove."
fi
#!/bin/bash
# install.sh
# Installs the 'matfun' matrix calculator on Linux.
#
# Usage:
#   # Quick one-liner:
#   curl -sSL https://raw.githubusercontent.com/RG8420/matfun/main/install.sh | bash
#
#   # Or clone and run locally:
#   git clone https://github.com/RG8420/matfun.git
#   cd matfun
#   bash install.sh
#
# Options:
#   --uninstall  Remove matfun from system

set -eo pipefail

REPO_URL="https://github.com/RG8420/matfun.git"
INSTALL_DIR="$HOME/.local/bin"
SYSTEM_INSTALL_DIR="/usr/local/bin"

usage() {
    echo "Usage: install.sh [--uninstall]"
    echo "  --uninstall  Remove matfun from system"
    exit 1
}

uninstall_matfun() {
    echo "=== Matfun Uninstaller ==="

    local targets=("$SYSTEM_INSTALL_DIR/matfun" "$INSTALL_DIR/matfun")
    local removed=0

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
    exit 0
}

if [[ "${1:-}" == "--uninstall" ]]; then
    uninstall_matfun
fi

if [[ "${1:-}" == "-h" ]] || [[ "${1:-}" == "--help" ]]; then
    usage
fi

echo "=== Matrix Calculator (matfun) Installer ==="

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [[ "$SCRIPT_DIR" == *"/tmp"* ]] || [[ ! -f "$SCRIPT_DIR/Makefile" ]]; then
    echo "[*] Detected run from curl/download - cloning repository..."
    TEMP_DIR=$(mktemp -d)
    git clone --depth 1 "$REPO_URL" "$TEMP_DIR"
    cd "$TEMP_DIR"
    SCRIPT_DIR="$TEMP_DIR"
    CLEANUP_TEMP=1
else
    CLEANUP_TEMP=0
fi

echo "[1/3] Building matfun..."
cd "$SCRIPT_DIR"

if ! command -v gcc &> /dev/null; then
    echo "  Error: GCC not found. Please install GCC."
    exit 1
fi

make clean 2>/dev/null || true
make

echo "[2/3] Installing matfun..."

if [ -w "$SYSTEM_INSTALL_DIR" ]; then
    cp matfun "$SYSTEM_INSTALL_DIR/matfun"
    chmod +x "$SYSTEM_INSTALL_DIR/matfun"
    echo "  Installed to $SYSTEM_INSTALL_DIR (system-wide)"
    INSTALL_TARGET="$SYSTEM_INSTALL_DIR/matfun"
else
    if [ ! -d "$INSTALL_DIR" ]; then
        mkdir -p "$INSTALL_DIR"
    fi
    cp matfun "$INSTALL_DIR/matfun"
    chmod +x "$INSTALL_DIR/matfun"
    echo "  Installed to $INSTALL_DIR (user-level)"
    INSTALL_TARGET="$INSTALL_DIR/matfun"
fi

echo "[3/3] Configuring PATH..."

SHELL_RC="$HOME/.bashrc"
if [ -n "${ZSH_VERSION:-}" ]; then
    SHELL_RC="$HOME/.zshrc"
fi

if [[ "$INSTALL_TARGET" == *"$INSTALL_DIR"* ]]; then
    if ! grep -q "$INSTALL_DIR" "$SHELL_RC" 2>/dev/null; then
        echo "" >> "$SHELL_RC"
        echo "# Matrix Calculator (matfun)" >> "$SHELL_RC"
        echo "export PATH=\"$INSTALL_DIR:\$PATH\"" >> "$SHELL_RC"
        echo "  Added $INSTALL_DIR to PATH in $SHELL_RC"
        echo "  Please restart your terminal or run: source $SHELL_RC"
    fi
fi

echo ""
echo "  Installation successful!"
echo ""
echo "  Run 'matfun' to start!"
echo ""
echo "  To uninstall, run:  bash install.sh --uninstall"

if [ "$CLEANUP_TEMP" -eq 1 ]; then
    rm -rf "$TEMP_DIR"
fi
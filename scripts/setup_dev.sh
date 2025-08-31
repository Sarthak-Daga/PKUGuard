#!/usr/bin/env bash
set -e
# Usage: bash scripts/setup_dev.sh
# Installs dev packages on Debian/Ubuntu (run in your WSL2 distro)
sudo apt update
sudo apt install -y build-essential flex bison libncurses-dev libelf-dev \
    libssl-dev ccache python3 git pkg-config qemu-system-x86 qemu-utils \
    debootstrap
echo "Dev dependencies installed. To build kernels you'll still need more steps (see docs)."


#!/usr/bin/env bash
# Usage:
#   scripts/run_qemu.sh <disk-image.qcow2> [kernel-bzImage]
# This template boots with an Intel Skylake CPU model which exposes PKU flags.
set -e
IMG=${1:-disk.qcow2}
KERNEL=${2:-""}

# Basic qemu command (TCG fallback if KVM is unavailable - remove accel if it errors)
if [ -n "$KERNEL" ]; then
  qemu-system-x86_64 \
    -machine accel=kvm:tcg \
    -cpu Skylake-Client \
    -smp 4 -m 4096 \
    -kernel "$KERNEL" \
    -append "root=/dev/vda1 console=ttyS0 rw" \
    -drive file="$IMG",format=qcow2,if=virtio \
    -nographic
else
  # Boot disk image (assumes it has bootloader & OS installed)
  qemu-system-x86_64 \
    -machine accel=kvm:tcg \
    -cpu Skylake-Client \
    -smp 4 -m 4096 \
    -drive file="$IMG",format=qcow2,if=virtio \
    -nographic
fi


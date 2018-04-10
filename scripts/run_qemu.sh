#!/bin/bash

# run_qemu.sh -- run QEMU emulator mounting the kernel's ISO
# Usage: run_qemu.sh

qemu-system-i386 -cdrom min-os.iso

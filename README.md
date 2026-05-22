# HelloEfi

Minimal UEFI application written in C,
bootable in QEMU using OVMF firmware.

This project explores:
- UEFI applications
- low-level boot environments
- firmware interfaces
- QEMU-based system emulation



# Screenshot
Bootscreen in QEMU


# Architektur
Host Linux
	|
Build EFI Binary
	|
QEMU + OVMF
	|
UEFI Application

# Build Instructions
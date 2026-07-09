---
title: Getting Started
---

# Getting Started

This guide explains how to build and run the UEFI Playground locally using QEMU and OVMF.

## Requirements

The project is developed and tested on Ubuntu/Debian based Linux systems.

Required tools:

- GCC
- GNU-EFI
- GNU binutils
- QEMU
- OVMF firmware

Install dependencies:

```bash
sudo apt install ovmf qemu-system-x86 gnu-efi
```

## Clone the Repository

Clone the repository:

```bash
git clone https://github.com/koenigr/uefi-playground.git
cd uefi-playground
```

## Build and Run

Build all EFI applications and start QEMU:

```bash
make run
```

This will:
1. Compile the UEFI applications
2. Create EFI binaries
3. Start QEMU with OVMF firmware
4. Boot into the UEFI environment

## QEMU and OVMF

The project uses OVMF to provide a virtual UEFI firmware environment.

The boot process looks like this:
```text
QEMU
 |
 +--> OVMF Firmware
          |
          +--> UEFI Applications
          |
          +--> Boot Loader
```

This allows development and debugging without modifying physical hardware.

## UEFI Shell

When the UEFI shell is available, common commands are:

Switch to the EFI filesystem:
```
fs0:
```

List files:
```
ls
```

Exit the shell:
```
exit
```

## Running Applications

The generated EFI binaries are located in:

```text
build/
├── apps/
│   ├── hello.efi
│   ├── memory_map.efi
│   ├── filesystem.efi
│   └── graphics.efi
└── boot/
```

Applications can be launched from the UEFI shell.

Example:
```text
fs0:
hello.efi
```

## Development Environment

The project intentionally uses a minimal toolchain:
- C for implementation
- GNU-EFI for UEFI interfaces
- GCC and binutils for compilation
- QEMU and OVMF for testing

The goal is to understand the firmware environment directly rather than hiding it behind higher-level frameworks.
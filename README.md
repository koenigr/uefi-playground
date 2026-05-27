# Uefi-Playground

Minimal UEFI application written in C,
bootable in QEMU using OVMF firmware.

This project explores:
- UEFI applications
- low-level boot environments
- firmware interfaces
- QEMU-based system emulation

Main goal is to see differences between a simple Application inside UEFI and a bootable Executable.

# Resources/ Documentation

https://uefi.org  
https://wiki.osdev.org/UEFI  
https://sourceforge.net/projects/gnu-efi/  

## UEFI 2.11 Specification
https://uefi.org/sites/default/files/resources/UEFI_Spec_Final_2.11.pdf


# UEFI Application vs Bootable Firmware Entry

UEFI distinguishes between *applications* and *bootable loaders*, even though both are compiled as `.efi` binaries.

## UEFI Application
A UEFI application is executed by an already running UEFI firmware environment.

- Started manually from the UEFI shell or boot manager
- Runs with full access to Boot Services
- Uses the existing system context (no system takeover)
- Example: memory mao viewer, filesystem tools, diagnostics

## Bootable EFI Loader
A bootable EFI image acts as a **boot entry point for an operating system**.

- Executed automatically by the firmware during boot
- Responsible for preparing system handoff to an OS
- Typically loads a kernel or continues the boot chain
- Runs early in the boot process before ExitBootServices()

## Key Difference
The main idfference is **role in the boot process**:
- Applications run *on top of firmware*
- Bootable loaders *transition from firmware to OS*

Both use the same UEFI interfaces, but operate at different stages of system initialization.

## Lerning Focus of this Project
This project is intentionally focused on low-level firmware behaviour and UEFI-specific details, including:

- Boot Services lifecycle and constraints
- Memory management via 'GetMemoryMap'
- Calling conventions and API details (e.g. stack alignment issues)
- Differences between firmware execution contexts and OS handoff
- Debugging behaviour in QEMU/OVMF environments

A key goal is to understand subtle implementation details that are often abstracted away in higher-level systems programmin, such as why certain UEFI calls my behave differently depending on parameter layout or compiler/API configuration.

# Screenshot
Bootscreen in QEMU

![UEFI Boot in QEMU showing HelloEfi output](assets/Running.jpg)


# Architecture
Host Linux  ->  Build EFI Binary  ->  QEMU + OVMF  ->  UEFI Application  

# Dependencies

Ubuntu/Debian:
```bash
sudo apt install ovmf qemu-system-x86 gnu-efi 
```

# Build Instructions
```bash
make run
```

# UEFI Interactive Shell - commands
Switch HD: ```fs0:```  
List files: ```ls```  
To leave shell: ```exit```  

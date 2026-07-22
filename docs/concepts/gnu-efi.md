---
title: GNU-EFI
---

# GNU-EFI

## Overview

GNU-EFI is a development library and toolchain support package that allows UEFI applications to be written in C using GCC.

It provides:

- UEFI headers
- Type definitions
- Helper macros
- Startup code
- Build support for generating `.efi` binaries

This project uses GNU-EFI together with GCC, GNU ld and QEMU/OVMF.

---

## Why GNU-EFI?

UEFI applications require:

- A specific executable format
- UEFI-compatible calling conventions
- Firmware type definitions
- Access to UEFI service interfaces

GNU-EFI provides these components while allowing development with the standard GNU toolchain.

---

## Basic Application Structure

A minimal GNU-EFI application looks like:

```c
#include <efi.h>
#include <efilib.h>

EFI_STATUS
efi_main(EFI_HANDLE ImageHandle,
         EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);

    Print(L"Hello, UEFI!\n");

    return EFI_SUCCESS;
}
```

The firmware loads the `.efi` image and calls `efi_main()`.

---

## Key Headers

Common headers used in this project:

| Header | Purpose |
|--------|---------|
| `<efi.h>` | Core UEFI types and definitions |
| `<efilib.h>` | Helper functions and library utilities |

---

## Building EFI Binaries

GNU-EFI applications are not ordinary Linux executables.

The build process typically involves:

1. Compiling C code
2. Linking with GNU-EFI libraries
3. Producing a PE/COFF EFI image
4. Renaming the output to `.efi`

The project automates this process through the Makefile.

---

## The UEFI Calling Convention

UEFI uses a firmware-defined calling convention.

Functions exposed by the firmware are declared with the `EFIAPI` macro.

Example:

```c
EFI_STATUS EFIAPI MyFunction(void);
```

Using the correct calling convention is essential when interacting with firmware services.

---

## Boot Services Access

GNU-EFI exposes the firmware interfaces through the UEFI System Table.

Example:

```c
SystemTable->BootServices
```

This gives access to functions such as:

- `GetMemoryMap()`
- `AllocatePool()`
- `FreePool()`
- `LocateProtocol()`

---

## A Subtle ABI Issue

During development of the Memory Map Viewer, direct Boot Services calls caused unexpected failures on x86_64.

Example:

```c
SystemTable->BootServices->GetMemoryMap(...)
```

Using the GNU-EFI helper:

```c
uefi_call_wrapper(...)
```

resolved the issue.

This suggests an ABI or calling-convention related interaction between GCC-generated code and firmware service calls.

The exact root cause requires further investigation, but the finding highlights that firmware interfaces are more sensitive to ABI details than ordinary user-space APIs.

---

## Why This Matters

GNU-EFI is intentionally lightweight.

It exposes the UEFI interfaces relatively directly, which makes it well suited for learning:

- Firmware execution environments
- Boot Services
- Protocol-based interfaces
- Memory management
- OS handoff mechanics

This is one of the main reasons it was chosen for this project.

---

## Related Concepts

- [Boot Services](boot-services.md)
- [UEFI Protocols](protocols.md)
- [ExitBootServices](exitbootservices.md)

---

## Related Findings

- [ABI and Calling Convention Findings](../findings/abi.md)
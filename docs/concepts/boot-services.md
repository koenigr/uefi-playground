---
title: UEFI Boot Services
nav_order: 1
parent: Concepts
---

# UEFI Boot Services

## Overview

Boot Services are a collection of UEFI functions provided by the firmware while the system is still under firmware control.

They allow UEFI applications and boot loaders to perform tasks such as memory allocation, protocol discovery, event handling and loading additional EFI images.

Boot Services are available until the operating system loader calls `ExitBootServices()`.

---

## Role in the Boot Process

During early system startup, the firmware provides an execution environment for EFI applications.

The simplified flow is:
```text
    Firmware starts
          |
          v
    UEFI Boot Services available
          |
          v
    EFI Application / Boot Loader runs
          |
          v
    ExitBootServices()
          |
          v
    Operating system takes control
```

After `ExitBootServices()` returns successfully, the firmware releases control over hardware resources managed through Boot Services.

---

## Accessing Boot Services

Boot Services are accessed through the UEFI System Table:

```text
    EFI_SYSTEM_TABLE
            |
            v
    EFI_BOOT_SERVICES
```
The system table is provided by the firmware when the application starts.

Example:

```c
SystemTable->BootServices
```

This structure contains pointers to the available Boot Service functions.

---

## Boot Services Used in This Project

### Memory Management

The Memory Map Viewer uses:

- `GetMemoryMap()`
- `AllocatePool()`
- `FreePool()`

These functions allow the application to inspect and manage memory before the operating system starts.

---

### Protocol Discovery

Several applications use:

- `LocateProtocol()`

This allows applications to find firmware interfaces dynamically.

Examples:

- Simple File System Protocol
- Graphics Output Protocol

---

### File and Graphics Access

The Filesystem Explorer and Graphics Demo use Boot Services to locate the required UEFI protocols before interacting with hardware.

---

## Boot Services Lifetime

Boot Services are temporary.

They exist only during the firmware phase.

A boot loader typically uses them to:

- Load the operating system kernel
- Prepare memory
- Retrieve the final memory map
- Configure hardware

Before transferring control to the operating system, the boot loader calls:

`ExitBootServices()`

After that point:

- Boot Services are no longer available
- The operating system manages the hardware directly

---

## Related Concepts

- [Memory Map](memory-map.md)
- [UEFI Protocols](protocols.md)
- [ExitBootServices](exitbootservices.md)

---

## Project Usage

This project uses Boot Services extensively to explore the environment before an operating system exists.

Understanding Boot Services is essential for the later goal of implementing a Linux EFI loader.
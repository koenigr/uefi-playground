---
title: ExitBootServices
nav_order: 6
parent: Concepts
---

# ExitBootServices

## Overview

`ExitBootServices()` is the UEFI function that ends the firmware boot phase and transfers control of the system to the operating system loader.

After this call succeeds, most UEFI Boot Services become unavailable.

This is the critical transition point between **firmware control** and **operating system control**.

---

## Why It Exists

During startup, firmware provides services such as:

- Memory allocation
- Protocol discovery
- File access
- Event handling
- Image loading

An operating system cannot continue using these firmware services indefinitely because it must manage memory and hardware itself.

`ExitBootServices()` cleanly terminates the firmware-managed environment.

---

## Boot Process Context

A simplified boot sequence:

    Power on
        |
        v
    Firmware initialization
        |
        v
    Boot Services available
        |
        v
    EFI application / boot loader
        |
        v
    Get final memory map
        |
        v
    ExitBootServices()
        |
        v
    Operating system takes control

The call is usually performed immediately before jumping to the kernel entry point.

---

## Function Signature

The firmware exposes:

```c
EFI_STATUS
(EFIAPI *ExitBootServices)(
    EFI_HANDLE ImageHandle,
    UINTN MapKey
);
```

Two parameters are required:

- `ImageHandle` — the handle of the current EFI image
- `MapKey` — the key returned by the **final successful** `GetMemoryMap()` call

---

## The MapKey Requirement

This is the most important rule.

The boot loader must:

1. Call `GetMemoryMap()`
2. Receive a `MapKey`
3. Avoid changing the memory map
4. Call `ExitBootServices()` with that exact key

If the memory map changes after retrieving the key, the call fails.

Example failure sequence:

    GetMemoryMap()  -> MapKey = 42
            |
            v
    AllocatePool() changes memory map
            |
            v
    ExitBootServices(ImageHandle, 42)
            |
            v
    EFI_INVALID_PARAMETER

This is why boot loaders retrieve the memory map **as late as possible**.

---

## What Happens After Success?

Once `ExitBootServices()` succeeds:

### No longer available

- `AllocatePool()`
- `FreePool()`
- `LocateProtocol()`
- File access through Boot Services
- Most firmware-managed services

### Still available

- CPU execution
- Physical memory
- Framebuffer memory (if configured)
- Runtime Services (through a separate mechanism)

At this point the kernel is responsible for managing the system.

---

## Why This Is Difficult

The transition is deceptively small in code:

```c
Status = ExitBootServices(ImageHandle, MapKey);
```

but it represents a major architectural change:

| Before | After |
|--------|-------|
| Firmware manages memory | Kernel manages memory |
| Boot Services available | Boot Services unavailable |
| Protocol discovery possible | No new protocols can be located |
| File access available | Loader must already have loaded required files |

A single additional allocation between `GetMemoryMap()` and `ExitBootServices()` can invalidate the MapKey.

---

## Relationship to This Project

The current applications run entirely **before** `ExitBootServices()`.

Examples:

- Memory Map Viewer
- Filesystem Explorer
- Graphics Demo

The planned Linux loader will be the first component that must:

1. Load the kernel
2. Retrieve the final memory map
3. Call `ExitBootServices()`
4. Jump to the kernel entry point

This makes `ExitBootServices()` the central milestone for the boot loader phase of the project.

---

## Related Concepts

- [Boot Services](boot-services.md)
- [Memory Map](memory-map.md)
- [GNU-EFI](gnu-efi.md)

---

## Related Findings

- [GetMemoryMap() Behaviour](../findings/getmemorymap.md)
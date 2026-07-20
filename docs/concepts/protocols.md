---
title: UEFI Protocols
---

# UEFI Protocols

## Overview

UEFI protocols are the main mechanism used by firmware to expose functionality to applications and boot loaders.

Instead of directly accessing hardware, UEFI applications locate protocols that provide standardized interfaces.

Examples include:

- Filesystem access
- Graphics output
- Device communication
- Loaded image information

---

## Protocol Concept

A protocol is a structure containing function pointers and related data.

An application does not need to know the hardware implementation behind a protocol.

The simplified workflow is:

    EFI Application
          |
          v
    LocateProtocol()
          |
          v
    UEFI Protocol Interface
          |
          v
    Firmware Implementation

This allows the same application to run on different firmware implementations.

---

## Locating a Protocol

Protocols are usually discovered through Boot Services:

    LocateProtocol()
            |
            v
    Protocol GUID
            |
            v
    Protocol Interface

The application provides a protocol identifier (GUID), and firmware returns a pointer to the corresponding interface.

---

## Protocols Used in This Project

## Simple File System Protocol

The Filesystem Explorer uses:

`EFI_SIMPLE_FILE_SYSTEM_PROTOCOL`

It provides access to EFI volumes and allows applications to:

- Open filesystem volumes
- Access the root directory
- Read and write files

Workflow:
```text
    LocateProtocol()
            |
            v
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
            |
            v
    OpenVolume()
            |
            v
    EFI_FILE_PROTOCOL
```
---

## Graphics Output Protocol

The Graphics Demo uses:

`EFI_GRAPHICS_OUTPUT_PROTOCOL`

It provides access to framebuffer-based graphics.

The application can:

- Query available modes
- Change graphics modes
- Access framebuffer memory
- Render pixels directly

---

## Why Protocols Matter

Protocols separate application logic from firmware implementation.

An application does not need to know:

- Which graphics hardware is installed
- Which filesystem implementation is used
- How the firmware communicates with devices

It only needs to use the defined protocol interface.

---

## Related Concepts

- [Boot Services](boot-services.md)
- [Graphics Output Protocol](graphics-output-protocol.md)
- [Memory Map](memory-map.md)

---

## Project Usage

Understanding protocols is essential for UEFI development.

The applications in this project use protocols to access functionality provided by the firmware before an operating system is running.
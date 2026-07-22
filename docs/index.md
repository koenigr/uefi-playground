---
title: UEFI Playground
nav_order: 1
---

# UEFI Playground

**Low-level firmware development in C**

A hands-on exploration of UEFI internals, boot processes, memory management and system initialization.

Built with:
- C
- GNU-EFI
- QEMU
- OVMF
- Linux

Modern operating systems hide many low-level details behind layers of abstraction.

This project explores what happens before an operating system starts:
how firmware initializes hardware, provides services, manages memory and transfers control to the next boot stage.

## Getting Started
 New to UEFI development or this project?

 Start here:
 [Getting Started](getting-started.md)

## Architecture

```text
Linux Host
    |
    v
QEMU + OVMF Firmware
    |
    +--> UEFI Applications
    |       |
    |       +--> Memory Map Viewer
    |       +--> Filesystem Explorer
    |       +--> Graphics Demo
    |
    +--> Boot Loader
            |
            +--> Linux Loader (planned)
```

## Learning Topics
- UEFI Applications and Boot Services
- Memory management via `GetMemoryMap()`
- Filesystem access through EFI protocols
- Graphics Output Protocol (GOP)
- Boot process transition (firmware -> OS)
- QEMU + OVMF emulation environment

## Applications

The project currently contains several standalone UEFI applications exploring different parts of the firmware environment.

---

### Memory Map Viewer

Explores how UEFI represents system memory before the operating system starts.

Features:
- Retrieves the firmware memory map via `GetMemoryMap()`
- Displays EFI memory descriptors
- Groups contiguous memory regions
- Calculates memory sizes

![Memory Map Viewer output](assets/MemoryMap.jpg)

[Read more about Memory Map Viewer](applications/memory-map-viewer.md)

---

### Filesystem Explorer

Demonstrates access to EFI filesystem protocols.

Features:
- Locates EFI Simple File System Protocol
- Opens filesystem volumes
- Enumerates directories
- Reads file metadata

![Filesystem Explorer output](assets/FilesystemExplorer.jpg)

[Read more about Filesystem Explorer](applications/filesystem-explorer.md)

---

### Graphics Output Protocol Demo

Explores direct framebuffer access using the UEFI Graphics Output Protocol.

Features:
- Retrieves framebuffer information
- Draws pixels directly into framebuffer memory
- Implements basic drawing primitives

![Graphics Output Protocol Demo](assets/GOPOutput.jpg)

[Read more about Graphics Demo](applications/graphics-demo.md)

---

### Hello World UEFI Application

The first application of the project.

It demonstrates:
- Creating a `.efi` executable
- Receiving the UEFI System Table
- Writing output through firmware services

![Hello World running in UEFI Shell](assets/Running.jpg)

[Read more about Hello World](applications/hello.md)

## Core Concepts

- [Boot Services](concepts/boot-services.md)
- [Memory Map Documentation](concepts/memory-map.md)
- [Protocols](concepts/protocols.md)
- [ExitBootServices](concepts/exit-boot-services.md)
- [Graphics Output Protocol](concepts/graphics-output-protocol.md)

## Technical Findings

During development several implementation details turned out to be less obvious than expected.

Topics include:

- [Memory ownership in UEFI APIs](findings/memory-ownership.md)
- [GetMemoryMap() behaviour](findings/getmemorymap.md)
- [Descriptor iteration using DescriptorSize](findings/getmemorymap.md)
- [ABI compatibility with GNU-EFI](findings/abi.md)

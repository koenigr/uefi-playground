---
title: UEFI Playground
nav_order: 1
---

# UEFI Playground
A hands-on learning project exploring **UEFI firmware development**, low-level boot processes, and system initialization.

This project is not about abstraction - it is about understanding what happens *before an operating system exists*.

## Getting Started
 New to UEFI development or this project?

 Start here:
 [Getting Started](getting-started.md)

## Learning Topics
- UEFI Applications and Boot Services
- Memory management via `GetMemoryMap()`
- Filesystem access through EFI protocols
- Graphics Output Protocol (GOP)
- Boot process transition (firmware -> OS)
- QEMU + OVMF emulation environment

## Applications

| Application | Description |
|-------------|-------------|
| [Hello World](applications/hello.md) | First UEFI application |
| [Memory Map Viewer](applications/memory-map-viewer.md) | Explore the firmware memory map |
| [Filesystem Explorer](applications/filesystem-explorer.md) | Browse EFI volumes |
| [Graphics Demo](applications/graphics-demo.md) | Draw directly to the framebuffer |

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
- [ABI compatibility with GNU-EFI](findings/api.md)

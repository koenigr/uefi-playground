---
title: Graphics Demo
nav_order: 4
parent: Applications
---

# Graphics Demo

## Overview

The Graphics Demo is a UEFI application that uses the **Graphics Output Protocol (GOP)** to switch graphics modes and render directly into the framebuffer.

Unlike previous applications that only used text console output, this application performs pixel-level graphics rendering before an operating system is loaded.

---

## Goal

The goal of this application is to understand how UEFI exposes graphics hardware through GOP and how firmware applications can draw directly into video memory.

---

## What it demonstrates

- Locating `EFI_GRAPHICS_OUTPUT_PROTOCOL`
- Querying available graphics modes
- Switching modes using `SetMode()`
- Accessing framebuffer memory
- Plotting individual pixels
- Drawing lines and rectangles
- Separating console output from graphical rendering

---

## GOP Workflow

The application follows this sequence:

```text
    LocateProtocol()
            |
            v
    EFI_GRAPHICS_OUTPUT_PROTOCOL
            |
            v
    QueryMode()
            |
            v
    SetMode()
            |
            v
    Access framebuffer
            |
            v
    Render pixels and shapes
```
---

## Framebuffer Rendering

After selecting a graphics mode, GOP provides:

- Framebuffer base address
- Framebuffer size
- Resolution
- Pixel format
- Pixels-per-scan-line information

Rendering is performed by writing pixel values directly into framebuffer memory.

---

## Drawing Primitives

The demo implements several basic rendering primitives:

- `PutPixel()`
- Horizontal lines
- Vertical lines
- Rectangle outlines
- Filled rectangles

These primitives are sufficient to build simple user interfaces and debugging overlays.

---

## Console vs Graphics

A useful observation is that text console output and framebuffer rendering are not the same subsystem.

The application reserves a region of the screen for graphics rendering while keeping console messages visible in another area.

This demonstrates the separation between:

- UEFI text console services
- GOP framebuffer rendering

---

## Memory Ownership

An important detail discovered during development concerns `QueryMode()`.

The function returns a buffer allocated by the graphics driver.

This means the caller must release the memory with `FreePool()` after the mode information is no longer needed.

This differs from APIs such as `GetMemoryMap()`, where the caller allocates the buffer itself.

---

## Output

Example framebuffer rendering:

![Graphics Output Demo](../assets/GOPOutput.jpg)

The demo draws colored rectangles and lines directly into video memory while running inside QEMU with OVMF firmware.

---

## Related Concepts

- [UEFI Protocols](../concepts/protocols.md)
- [Boot Services](../concepts/boot-services.md)
- [Graphics Output Protocol](../concepts/graphics-output-protocol.md)
- [Memory Ownership](../findings/memory-ownership.md)

---

## Next Steps

The next stage of the project moves from standalone applications to boot loader development.

[Boot Loader Overview](../bootloader/overview.md)
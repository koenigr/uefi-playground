---
---

<link rel="stylesheet" href="/assets/css/style.css">

# UEFI Playground
A hands-on learning project exploring **UEFI firmware development**, low-level boot processes, and system initialization.

This project is not about abstraction - it is about understanding what happens *before an operating system exists*.

## What this project explores
- UEFI Applications and Boot Services
- Memory management via `GetMemoryMap()`
- Filesystem access through EFI protocols
- Graphics Output Protocol (GOP)
- Boot process transition (firmware -> OS)
- QEMU + OVMF emulation environment

# Current Status
## Completed
- Hello World UEFI application
- Memory Map Viewer
- Filesystem Eplorer

## In Progress
- Graphics Output Protocol (GOP) demo

## Planned
- Linux kernel loader
- `ExitBootServices()` handoff
- GDB debugging integration

## Project Status
[See whole description of project status](Project-Status.md)

## Documentation

- Memory Mao
	- [Memory Map Documentation](docs/memory/memorymap_viewer.md) 
- Filesystem
	- [EFI_FILE_INFO structure and allocation](docs/filesystem/efi_file_info.md)

## Roadmap

- Graphics Output Protocol
- Linux Loader
- ExitBootServices

# UEFI Playground - Project Status

## Goal

Learn UEFI development and low-level boot processes by implementing small standalone UEFI applications and a bootable EFI loader.

The project focuses on understanding firmware behaviour, boot services, memory management, filesystem access, graphics output, and the transition from firmware to operating system.

---

## Environment

### Host
- Linux (Ubuntu/Debian based)

### Toolchain
- GNU-EFI
- GCC
- GNU ld
- objcopy

### Virtualization
- QEMU
- OVMF firmware

### Build

```bash
make run
```

Builds all EFI binaries and starts QEMU with OVMF.

---

## Repository Structure

```text
src/
├── apps/
│   ├── hello/
│   ├── memory_map/
│   ├── graphics/
│   └── filesystem/
│
└── boot/
    ├── template/
    └── linux_loader/

build/
```

---

## Completed Applications

### Hello World

Status: Complete

Features:
- Basic UEFI application
- Uses GNU-EFI
- Prints information to console

---

### Memory Map Viewer

Status: Complete (v1)

Features:
- Calls GetMemoryMap()
- Allocates memory via AllocatePool()
- Retrieves full EFI memory map
- Iterates over EFI_MEMORY_DESCRIPTOR entries
- Displays:
  - Memory type
  - Physical start address
  - Page count
  - Calculated size
- Converts EFI memory type enums to readable names
- Groups contiguous memory regions of the same type
- Color-coded output

Important implementation details:
- Requires two GetMemoryMap() calls
- AllocatePool() changes the memory map
- Additional descriptor space is allocated before second GetMemoryMap()
- Memory map buffer is freed with FreePool()

---

### Filesystem Explorer

Goal:
- Access EFI Simple File System Protocol
- Open volumes
- Enumerate directories
- Read files


## Planned Applications

---

### Graphics Demo

Goal:
- Access Graphics Output Protocol (GOP)
- Query available modes
- Draw pixels/shapes
- Display framebuffer information

---

## Boot Loader

### Linux Loader

Status: Planned

Long-term goals:
- Load kernel image
- Retrieve memory map
- ExitBootServices()
- Transfer control to kernel

---

## Important Technical Findings

### GNU-EFI x86_64

Using direct BootServices calls caused failures.

Example:

```c
SystemTable->BootServices->GetMemoryMap(...)
```

was unreliable.

Using:

```c
uefi_call_wrapper(...)
```

resolved the issue.

Current assumption:
- Calling convention / ABI mismatch
- Stack alignment related behaviour

Further investigation may be useful later.

---

### UEFI API Memory Ownership Contracts (Caller vs. Callee)

When working with UEFI protocols, it is not sufficient to rely on function signatures alone to determine memory ownership. The UEFI specification must be consulted to understand whether a buffer is allocated by the caller or the callee.

Example: `EFI_GRAPHICS_OUTPUT_PROTOCOL.QueryMode()`

The function signature alone does not specify ownership:

```c
EFI_STATUS
(EFIAPI *QueryMode)(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    UINT32 ModeNumber,
    UINTN *SizeOfInfo,
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info
);
```

Only the specification clarifies that Info is a **callee-allocated buffer**:

```
"A pointer to a callee allocated buffer that returns information about ModeNumber."
```

This means:

- The graphics driver allocates the memory internally
- The caller receives a pointer to this buffer
- The caller is responsible for freeing it using FreePool()

This is not always consistent across UEFI APIs. For example:

GetMemoryMap() requires the caller to allocate the buffer
QueryMode() allocates the buffer in the callee

Current finding:

- UEFI memory ownership rules are function-specific and must be verified in the specification or headers
- The function prototype alone is insufficient to determine allocation responsibility

Example implication:

```c
// QueryMode(): callee allocates memory
Status = gop->QueryMode(..., &Info);
FreePool(Info); // required

// GetMemoryMap(): caller allocates memory
Status = BootServices->GetMemoryMap(&Size, Buffer, ...);
```

Further development should always verify memory ownership rules in the UEFI specification before implementing buffer handling logic.

### GetMemoryMap Behaviour

Typical workflow:

1. Call GetMemoryMap() with NULL buffer
2. Receive EFI_BUFFER_TOO_SMALL
3. Allocate buffer
4. Call GetMemoryMap() again
5. Iterate descriptors

Important:
- AllocatePool() modifies the memory map
- MemoryMapSize should be increased before allocation

Example:

```c
MemoryMapSize += 2 * DescriptorSize;
```

---

### EFI Memory Descriptor Iteration

Descriptors are iterated using DescriptorSize:

```c
Desc = (EFI_MEMORY_DESCRIPTOR*)
(
    (UINT8*)Desc + DescriptorSize
);
```

Do not use:

```c
Desc++
```

because descriptor size is firmware-defined.

---

### Debugging Approach

Currently used:
- Print()
- EFI_STATUS output via %r
- QEMU
- OVMF

Planned:
- Serial logging
- GDB debugging

---

## Current Coding Style

Preferences:

- Small standalone applications
- Explicit error handling
- Readable code over abstraction
- Refactor only after functionality works
- Learn underlying firmware concepts before optimizing

---

## GitHub Workflow

Current workflow:

1. Create feature branch
2. Implement feature
3. Create Pull Request
4. Merge into main
5. Delete branch

Using:
- GitHub Issues
- GitHub Milestones
- GitHub Projects

---

## Current Roadmap

Completed:
- Project setup
- QEMU + OVMF environment
- Hello World
- Memory Map Viewer
- Filesystem Explorer

Next:
- Graphics Output Protocol

Later:
- Linux Loader
- ExitBootServices()
- Kernel handoff
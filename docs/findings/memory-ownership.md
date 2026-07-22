---
title: UEFI Memory Ownership
nav_order: 2
parent: Findings
---

# UEFI Memory Ownership

## Overview

A recurring source of confusion during development was determining **who owns a memory buffer returned by a UEFI API**.

Some functions expect the **caller** to allocate the buffer, while others allocate memory internally and expect the **caller** to free it afterwards.

The function signature alone is often insufficient to determine the ownership model.

---

## Initial Assumption

A common assumption is:

> If a function receives a pointer-to-pointer, the caller probably allocates the memory.

This turned out to be incorrect for several UEFI interfaces.

---

## Caller-Allocated Example: GetMemoryMap

`GetMemoryMap()` requires the caller to provide the buffer.

Simplified usage:

```c
Status = GetMemoryMap(
    &Size,
    Buffer,
    &MapKey,
    &DescriptorSize,
    &Version
);
```

The workflow is:

1. Query required size
2. Allocate buffer with `AllocatePool()`
3. Call `GetMemoryMap()` again
4. Free the buffer when finished

Ownership:

- Allocation: **caller**
- Freeing: **caller**

---

## Callee-Allocated Example: QueryMode

`EFI_GRAPHICS_OUTPUT_PROTOCOL.QueryMode()` behaves differently.

Signature:

```c
EFI_STATUS
(EFIAPI *QueryMode)(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
    UINT32 ModeNumber,
    UINTN *SizeOfInfo,
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info
);
```

At first glance, the ownership is not obvious.

The UEFI specification clarifies that:

> `Info` is a callee-allocated buffer.

This means:

- The graphics driver allocates the memory internally.
- The caller receives the pointer.
- The caller must release it using `FreePool()`.

Correct usage:

```c
EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info = NULL;

Status = gop->QueryMode(gop, Mode, &InfoSize, &Info);

if (!EFI_ERROR(Status)) {
    /* use Info */
    FreePool(Info);
}
```

---

## Why the Signature Is Misleading

Both APIs involve pointers, but they follow different ownership contracts.

| API | Allocation | Freeing |
|-----|------------|----------|
| `GetMemoryMap()` | Caller | Caller |
| `QueryMode()` | Callee | Caller |

Without consulting the specification, both could easily be implemented incorrectly.

---

## Practical Rule

When implementing a new UEFI API call:

1. Read the function signature.
2. Read the UEFI specification section for that function.
3. Determine whether the buffer is:
   - caller-allocated
   - callee-allocated
   - static / firmware-owned

Never infer ownership from the C prototype alone.

---

## Why This Matters

Incorrect ownership handling can cause:

- Memory leaks
- Double frees
- Invalid `FreePool()` calls
- Corrupted firmware state
- Failures during boot services operations

These bugs are often harder to diagnose than ordinary user-space memory errors because they occur inside the firmware execution environment.

---

## Key Takeaways

- UEFI memory ownership is **function-specific**.
- Pointer-to-pointer parameters do not automatically imply caller allocation.
- `GetMemoryMap()` is caller-allocated.
- `QueryMode()` is callee-allocated.
- The specification must be consulted to determine the correct ownership model.

This finding significantly changed the approach used when implementing new UEFI protocol interactions in the project.
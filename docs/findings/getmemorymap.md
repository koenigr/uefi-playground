---
title: GetMemoryMap Behaviour
---

# GetMemoryMap Behaviour

## Overview

While implementing the Memory Map Viewer, a subtle but important behaviour of `GetMemoryMap()` became apparent: **allocating memory for the memory map changes the memory map itself**.

This creates a dependency between buffer allocation and the validity of the retrieved map.

---

## Initial Assumption

A straightforward implementation seemed sufficient:

1. Call `GetMemoryMap()` with a `NULL` buffer
2. Receive `EFI_BUFFER_TOO_SMALL`
3. Allocate the required buffer
4. Call `GetMemoryMap()` again

Pseudo-code:

```c
Status = GetMemoryMap(&Size, NULL, &MapKey, &DescSize, &Version);

AllocatePool(Size, &Buffer);

Status = GetMemoryMap(&Size, Buffer, &MapKey, &DescSize, &Version);
```

---

## The Problem

The second call occasionally failed even though the buffer had the reported size.

The reason is that `AllocatePool()` itself creates additional memory descriptors, which modifies the memory map between the two calls.

In other words:

    GetMemoryMap()
            |
            v
    Required size = X
            |
            v
    AllocatePool(X)
            |
            v
    Memory map changes
            |
            v
    GetMemoryMap() may require a larger buffer

---

## Practical Solution

The implementation reserves additional descriptor space before allocation.

Example:

```c
MemoryMapSize += 2 * DescriptorSize;
```

This provides enough room for the descriptors introduced by the allocation itself.

After applying this change, the retrieval became reliable.

---

## Why This Happens

`AllocatePool()` is a Boot Service.

Boot Services manage firmware memory regions, so allocating memory necessarily updates the internal memory map maintained by the firmware.

The operation is therefore not merely "getting a buffer"; it changes the state being queried.

---

## Descriptor Iteration Pitfall

Another issue encountered during implementation involved descriptor iteration.

Incorrect:

```c
Descriptor++;
```

Correct:

```c
Descriptor =
    (EFI_MEMORY_DESCRIPTOR*)
    (
        (UINT8*)Descriptor + DescriptorSize
    );
```

The descriptor size is firmware-defined and must not be assumed to match the C structure size.

---

## Connection to ExitBootServices

This behaviour becomes even more important for a boot loader.

A successful `ExitBootServices()` call requires the **MapKey** returned by the **final successful** `GetMemoryMap()` invocation.

Any allocation performed after retrieving that key invalidates it.

The correct sequence is therefore:

    Load kernel
          |
          v
    GetMemoryMap()
          |
          v
    Receive final MapKey
          |
          v
    Do NOT change memory map
          |
          v
    ExitBootServices()

---

## Key Takeaways

- `GetMemoryMap()` requires a two-step workflow.
- `AllocatePool()` modifies the memory map.
- Extra descriptor space should be reserved before allocation.
- Descriptors must be iterated using `DescriptorSize`.
- The final `MapKey` is required for `ExitBootServices()`.

This finding turned a seemingly simple API call into a deeper investigation of firmware memory management and boot-loader correctness.
---
title: GNU-EFI ABI and Calling Convention
nav_order: 3
parent: Findings
---

# GNU-EFI ABI and Calling Convention

## Overview

During development of the Memory Map Viewer, an unexpected behaviour occurred when calling UEFI Boot Services directly.

The issue appeared to be related to the interaction between GNU-EFI, GCC generated code and the firmware calling convention on x86_64.

---

## The Problem

The direct function call:

```c
SystemTable->BootServices->GetMemoryMap(...)
```

caused unreliable behaviour.

The same function call through the GNU-EFI helper:

```c
uefi_call_wrapper(...)
```

worked correctly.

---

## Why This Is Interesting

UEFI firmware functions are not ordinary C library functions.

They are provided by firmware and must follow the calling convention defined by the UEFI specification.

On x86_64 systems, differences in:

- register usage
- stack alignment
- parameter passing
- ABI assumptions

can lead to invalid function calls even when the C syntax appears correct.

---

## Direct Calls vs GNU-EFI Wrapper

### Direct call

The compiler generates a normal function call based on the active compiler ABI.

Example:

```c
Status = SystemTable->BootServices->GetMemoryMap(...);
```

### GNU-EFI wrapper

GNU-EFI provides:

```c
uefi_call_wrapper(Function, Arguments, ...);
```

The wrapper ensures that the function call follows the expected UEFI calling convention.

---

## Possible Cause

The exact root cause was not fully investigated.

The current assumption is:

- ABI mismatch between generated code and firmware expectations
- Possible calling convention differences
- Possible stack alignment issues

Further investigation would require:

- Assembly inspection
- Compiler flags analysis
- Debugging with GDB
- Comparing generated call sequences

---

## Lessons Learned

This issue demonstrated that firmware development has additional constraints compared to normal application development.

A function prototype alone does not guarantee a valid function call.

When interacting with firmware interfaces, developers must consider:

- Platform ABI
- Calling conventions
- Compiler configuration
- Toolchain assumptions

---

## Relevance for This Project

This finding influenced the implementation style used throughout the project.

Firmware calls are treated carefully, especially when working with:

- Boot Services
- Protocol interfaces
- Low-level memory management

Understanding these details is essential for future boot loader development.
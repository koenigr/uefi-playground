---
title: Hello World UEFI Application
nav_order: 1
parent: Applications
---

# Hello World UEFI Application

## Overview

The Hello World application is the first standalone UEFI application in this project.

It demonstrates the basic structure of a UEFI executable, how firmware starts an application, and how an application interacts with the UEFI environment.

---

## Goal

The goal of this application is to create the smallest possible UEFI application using C and GNU-EFI.

The application runs directly inside the UEFI firmware environment without an operating system.

---

## What it demonstrates

- Creating a `.efi` executable
- Using GNU-EFI
- Receiving the UEFI System Table
- Writing output through UEFI services
- Running an application in QEMU with OVMF firmware

---

## UEFI Application Lifecycle

A simplified execution flow:

```text
UEFI Firmware

      |
      v

Load EFI application

      |
      v

efi_main()

      |
      v

Print output

      |
      v

Return EFI_STATUS

      |
      v

Application exits
```

Unlike a boot loader, the application does not take control of the system. It runs on top of the existing firmware environment.

## Implementation

The application entry point is:

```c
EFI_STATUS
efi_main(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
)
```

The firmware provides:

- The image handle of the application

- The system table containing access to UEFI services

The system table provides access to:

- Console output
- Boot Services
- Runtime Services
- Configuration Tables

## Running the Application

Build the project:

```bash
make
```

Start QEMU with OVMF:

```bash
make run
```

From the UEFI shell:

```
fs0:
hello.efi
```

The application will print its message to the UEFI console.

## Related Concepts

- [UEFI Protocols](../concepts/protocols.md)
- [Boot Services](../concepts/boot-services.md)
- [GNU-EFI](../concepts/gnu-efi.md)

## Next Steps

After understanding the basic application structure, the next step is exploring firmware memory management:

[Memory Map Viewer](memory-map-viewer.md)

Return to:
[Applications Overview](../index.md)


Eine kleine Anmerkung: Ich würde die Screenshots **nicht** auf jede Application-Seite packen. Die Startseite hat schon die Galerie. Bei den Detailseiten würde ich Screenshots nur hinzufügen, wenn sie etwas erklären (z. B. Memory Map mit farbiger Ausgabe oder GOP-Demo).



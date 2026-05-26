# Uefi-Playground

Minimal UEFI application written in C,
bootable in QEMU using OVMF firmware.

This project explores:
- UEFI applications
- low-level boot environments
- firmware interfaces
- QEMU-based system emulation

Main goal is to see differences between a simple Application inside UEFI and a bootable Executable.

# Explanation Difference between Application and Bootable

# Screenshot
Bootscreen in QEMU

![UEFI Boot in QEMU showing HelloEfi output](assets/Running.jpg)


# Architektur
Host Linux  
	|  
Build EFI Binary  
	|  
QEMU + OVMF  
	|  
UEFI Application  

# Dependencies

Ubuntu/Debian:
```bash
sudo apt install ovmf qemu-system-x86 gnu-efi 
```

# Build Instructions
```bash
make run
```

# UEFI Interactive Shell - commands
Switch HD: ```fs0:```
List files: ```ls```
To leave shell: ```exit```


# Applications
## Hello
Just a template how to create an application

## Memory Map Viewer
### Goal
Show which memory sections are reserved for the kernel before boot
### Procedure
Use GetMemoryMap() Boot Service  
You get an Array of EFI_MEMORY_DESCRIPTOR  
Number of Entries  
Size of Descriptor  
Map Key  
### Store Memory Map
UEFI requires two calls:
- get size
- allocate buffer
- get size
### Iterate over Entries
- Type (EfiLoaderCode, EfiConventionalMemory,...)
- Startaddress
- Pagenumber
- Attributes
### Format output
Type: EfiConventionalMemory
Start: 0x0000000000100000
Pages: 512
Size : 2 MB
### Translate Memory Types
- EfiReservedMemoryType
- EfiLoaderCode
- EfiLoaderData
- EfiBootServicesCode
- EfiBootServicesData
- EfiRuntimeServicesCode
- EfiRuntimeServicesData
- EfiConventionalMemory
### Optional
- Sorting/Grouping of related areas
- colored output
- get sizes: size = pages * 4096
# About Memory Map

## References

This chapter is based on the official UEFI specification, practical experiments performed in QEMU/OVMF, and the following reference material:

- **UEFI Specification 2.11**  
  https://uefi.org/specs/UEFI/2.11/index.html

- **Beyond BIOS: Developing with the Unified Extensible Firmware Interface, Third Edition**  
  Vincent Zimmer, Michael Rothman, Suresh Marisetty  
  De|G Press, 2017  
  ISBN-13: 978-1-5015-1478-4  
  DOI: 10.1515/9781501505690

- **TianoCore EDK II / OVMF**  
  https://github.com/tianocore/edk2

- **GNU-EFI Project**  
  https://sourceforge.net/projects/gnu-efi/

- **OSDev Wiki – UEFI**  
  https://wiki.osdev.org/UEFI

- **OSDev Wiki – Memory Map (x86)**  
  https://wiki.osdev.org/Memory_Map_(x86)


## Why does the firmware need a memory map
RAM ist nicht einfach "frei"  
Firmware reserviert Bereiche  
ACPI braucht Speicher  
MMIO ist kein normaler RAM  

## Procedure
Use GetMemoryMap() Boot Service  
You get an Array of EFI_MEMORY_DESCRIPTOR  
Number of Entries  
Size of Descriptor  
Map Key


### Why do we need DescriptorSize

Firmware darf Struktur erweitern  
sizeof(EFI_MEMORY_DESCRIPTOR) ist nicht die Wahrheit  
Deshalb Byte-weise iterieren  


## Store Memory Map
UEFI requires a two-step GetMemoryMap call:
- first call: get required buffer size
- allocate buffer
- second call: retrieve memory map

## Why two calls?

## Iterate over Entries / Layout of EFI_MEMORY_DESCRIPTOR
- Type (EfiLoaderCode, EfiConventionalMemory,...)
- PhysicalStart Address
- VirtualStart Address
- NumberOfPages count
- Flags / Attributes


## Translate Memory Types
- `EfiReservedMemoryType`
- `EfiLoaderCode`
- `EfiLoaderData`
- `EfiBootServicesCode`
- `EfiBootServicesData`
- `EfiRuntimeServicesCode`
- `EfiRuntimeServicesData`
- `EfiConventionalMemory`
- `EfiUnusableMemory`
- `EfiACPIReclaimMemory`
- `EfiACPIMemoryNVS`
- `EfiMemoryMappedIO`
- `EfiMemoryMappedIOPortSpace`
- `EfiPalCode` (rare)

Type | Meaning | Typical Use
---
EfiConventionalMemory | Free RAM | OS allocation
EfiLoaderCode | Loader | code	Boot loader
EfiBootServicesCode | Firmware code | UEFI services
EfiRuntimeServicesData | Runtime data | Variables, clocks
EfiMemoryMappedIO | MMIO | Devices


## TODO
Concepts to explain:
- GetMemoryMap()
- AllocatePool()
- ExitBootServices()
- Bootloader
- Kernel Handoff
- Descriptor Iteration
- Memory Ownership
- ABI-Problems

-> Application-Site of Memory Map Viewer, Findings to GetMemoryMap, ExitBootServices-Site


## Connection to ExitBootServices()


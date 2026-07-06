## Memory Map Viewer
### Goal
Show which memory sections are used or reserved before handing control to the operating system

### Procedure
Use GetMemoryMap() Boot Service  
You get an Array of EFI_MEMORY_DESCRIPTOR  
Number of Entries  
Size of Descriptor  
Map Key

### Store Memory Map
UEFI requires a two-step GetMemoryMap call:
- first call: get required buffer size
- allocate buffer
- second call: retrieve memory map

### Iterate over Entries
- Type (EfiLoaderCode, EfiConventionalMemory,...)
- Start Address
- Page count
- Flags / Attributes

### Format output
```text
Type: EfiConventionalMemory
Start: 0x0000000000100000
Pages: 512
Size : 2 MB
```
### Translate Memory Types
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

### Optional
- Sorting/Grouping of related areas
- colored output
- get sizes: size = pages * 4096


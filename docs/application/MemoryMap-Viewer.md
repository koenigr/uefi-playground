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
#include <efi.h>
#include <efilib.h>

/*
 * UINTN = Pointer-sized unsigned int
 */

CHAR16* MemoryTypeToStr(UINT32 Type) {
    switch (Type) {
        case EfiReservedMemoryType:        return L"Reserved";
        case EfiLoaderCode:                return L"LoaderCode";
        case EfiLoaderData:                return L"LoaderData";
        case EfiBootServicesCode:          return L"BootServicesCode";
        case EfiBootServicesData:          return L"BootServicesData";
        case EfiRuntimeServicesCode:       return L"RuntimeServicesCode";
        case EfiRuntimeServicesData:       return L"RuntimeServicesData";
        case EfiConventionalMemory:        return L"Conventional";
        case EfiUnusableMemory:            return L"Unusable";
        case EfiACPIReclaimMemory:         return L"ACPIReclaim";
        case EfiACPIMemoryNVS:             return L"ACPINVS";
        case EfiMemoryMappedIO:            return L"MMIO";
        case EfiMemoryMappedIOPortSpace:   return L"MMIOPort";
        case EfiPalCode:                   return L"PalCode";
        default:                           return L"Unknown";
    }
}

void SetColorForType(UINT32 Type, EFI_SYSTEM_TABLE *SystemTable) {
	switch(Type) {
		case EfiConventionalMemory:
	        SetColor(SystemTable, EFI_LIGHTGREEN);
	        break;

	    case EfiBootServicesCode:
	    case EfiBootServicesData:
	        SetColor(SystemTable, EFI_YELLOW);
	        break;

	    case EfiRuntimeServicesCode:
	    case EfiRuntimeServicesData:
	        SetColor(SystemTable, EFI_LIGHTCYAN);
	        break;

	    default:
	        SetColor(SystemTable, EFI_LIGHTRED);
	        break;
	}
}

void SetColor(EFI_SYSTEM_TABLE *SystemTable, UINTN Color)
{
	uefi_call_wrapper(
		SystemTable->ConOut->SetAttribute,
		2,
		SystemTable->ConOut,
		Color
	);
}

void PrintGroup(UINT32 Type, EFI_PHYSICAL_ADDRESS PhysicalStart, UINT64 NumberOfPages, EFI_SYSTEM_TABLE *SystemTable) {
	UINT64 SizeBytes = (UINT64) NumberOfPages * 4096;
	UINT64 SizeKB = SizeBytes / 1024;
	UINT64 SizeMB = SizeKB / 1024;
	SetColorForType(Type, SystemTable);

	Print(L"Type: %s\n", MemoryTypeToStr(Type));
	Print(L"Start: 0x%lx\n", PhysicalStart);
	Print(L"Pages: %lu\n", NumberOfPages);
	if (SizeMB > 0) {
		Print(L"Size: %lu MB\n", SizeMB);
	} else if (SizeKB > 0) {
		Print(L"Size: %lu KB\n", SizeKB);
	} else {
		Print(L"Size: %lu Bytes\n", SizeBytes);
	}
	Print(L"--------------------------------\n");

	SetColor(SystemTable, EFI_LIGHTGRAY);
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);
	Print(L"MemoryMap Viewer Application!\n");
	Print(L"Firmware Vendor: %s\n", SystemTable-> FirmwareVendor);
	Print(L"UEFI Revision: %d\n", SystemTable->Hdr.Revision);
	Print(L"\n");

	EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
	UINTN MemoryMapSize = 0;
	UINTN MapKey = 0;
	UINTN DescriptorSize = 0;
	UINT32 DescriptorVersion = 0;
	EFI_STATUS Status;

	Status = uefi_call_wrapper(
		SystemTable->BootServices->GetMemoryMap,
		5,
		&MemoryMapSize,
		NULL,
		&MapKey,
		&DescriptorSize,
		&DescriptorVersion
	);

	if (Status != EFI_BUFFER_TOO_SMALL) {
		Print(L"GetMemoryMap failed\n");
		return Status;
	} 

	MemoryMapSize += 2 * DescriptorSize;

	Status = uefi_call_wrapper(
		SystemTable->BootServices->AllocatePool,
		3,
		EfiLoaderData,
		MemoryMapSize,
		(void**)&MemoryMap
	);

	if (EFI_ERROR(Status)) {
		Print(L"AllocatePool failed: %r\n", Status);
		return Status;
	}

	Status = uefi_call_wrapper(
		SystemTable->BootServices->GetMemoryMap,
		5,
		&MemoryMapSize,
		MemoryMap,
		&MapKey,
		&DescriptorSize,
		&DescriptorVersion
	);

	if (EFI_ERROR(Status)) {
		Print(L"GetMemoryMap failed: %r\n", Status);
		return Status;
	}

	if (DescriptorSize == 0) {
		Print(L"DescriptorSize is 0!\n");
		return EFI_ABORTED;
	}

	/*
	 * GROUP PAGES BY TYPE
	 */

	EFI_MEMORY_DESCRIPTOR *Desc;

	EFI_MEMORY_TYPE currentType = 0;
	UINT64 currentStart = 0;
	UINT64 currentPages = 0;
	BOOLEAN first = TRUE;

	for (Desc = MemoryMap;
		(UINT8*)Desc < (UINT8*)MemoryMap + MemoryMapSize;
		Desc = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)Desc + DescriptorSize)) 
	{
		if (first) {
			currentType = Desc->Type;
			currentStart = Desc->PhysicalStart;
			currentPages = Desc->NumberOfPages;
			first = FALSE;
			continue;
		}

		UINT64 currentEnd = currentStart + currentPages * 4096;

		BOOLEAN contiguous = 
			(Desc->Type == currentType) &&
			(Desc->PhysicalStart == currentEnd);

		if (contiguous) {
			currentPages += Desc->NumberOfPages;
		} else {
			PrintGroup(currentType, currentStart, currentPages, SystemTable);

			currentType = Desc->Type;
			currentStart = Desc->PhysicalStart;
			currentPages = Desc->NumberOfPages;
		}
	}

	PrintGroup(currentType, currentStart, currentPages, SystemTable);

	Print(L"\n");
	Print(L"MemoryMapSize: %lx\n", MemoryMapSize);
	Print(L"DescriptorSize: %lu\n", DescriptorSize);

	uefi_call_wrapper(
		SystemTable->BootServices->FreePool,
		1,
		MemoryMap
	);

	return EFI_SUCCESS;
}
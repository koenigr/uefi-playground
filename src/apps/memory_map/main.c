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

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);
	Print(L"MemoryMap Viewer Application!\n");
	Print(L"SystemTable pointer: 0x%lx\n", (UINTN)SystemTable);
	Print(L"Firmware Vendor: %s\n", SystemTable-> FirmwareVendor);
	Print(L"UEFI Revision: %d\n", SystemTable->Hdr.Revision);
	Print(L"BootServices: 0x%lx\n", SystemTable->BootServices);

	Print(L"Testing BootServices...\n");

	SystemTable->BootServices->Stall(1000000);

	Print(L"BootServices Stall done\n");

	EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
	UINTN MemoryMapSize = 0;
	UINTN MapKey = 0;
	UINTN DescriptorSize = 0;
	UINT32 DescriptorVersion = 0;
	EFI_STATUS Status;

	Print(L"Declaration of variables done\n");

	volatile int x = 0;
	x++;

	Status = uefi_call_wrapper(
		SystemTable->BootServices->GetMemoryMap,
		5,
		&MemoryMapSize,
		NULL,
		&MapKey,
		&DescriptorSize,
		&DescriptorVersion
	);

	Print(L"After GMM: %r\n", Status);

	if (Status != EFI_BUFFER_TOO_SMALL) {
		Print(L"GetMemoryMap failed\n");
		return Status;
	} else {
		Print(L"GetMemoryMap successful\n");
	}

	Print(L"MemoryMapSize before AllocatePool: %lu\n", MemoryMapSize);
	Print(L"Status after GetMemoryMap: %r\n", Status);

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
	} else {
		Print(L"AllocatePool successful.\n");
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


	Print(L"MemoryMapSize: %d\n", MemoryMapSize);

	if (DescriptorSize == 0) {
    	Print(L"DescriptorSize is 0!\n");
    	return EFI_ABORTED;
	}

	for (
		EFI_MEMORY_DESCRIPTOR *Desc = MemoryMap;
		(UINT8*)Desc < (UINT8*)MemoryMap + MemoryMapSize;
		Desc = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)Desc + DescriptorSize)
	) {
		UINT64 SizeBytes = Desc->NumberOfPages * 4096;
		UINT64 SizeKB = SizeBytes / 1024;
		UINT64 SizeMB = SizeKB / 1024;

		Print(L"Type: %s\n", MemoryTypeToStr(Desc->Type));
		Print(L"Start: 0x%lx\n", Desc->PhysicalStart);
		Print(L"Pages: %lu\n", Desc->NumberOfPages);
		if (SizeMB > 0) {
			Print(L"Size: %lu MB\n", SizeMB);
		} else if (SizeKB > 0) {
			Print(L"Size: %lu KB\n", SizeKB);
		} else {
			Print(L"Size: %lu Bytes\n", SizeBytes);
		}
		Print(L"--------------------------------\n");
	}

	UINTN Count = MemoryMapSize / DescriptorSize;

	Print(L"MemoryMapSize: %lx\n", MemoryMapSize);
	Print(L"DescriptorSize: %d\n", DescriptorSize);
	Print(L"MapKey: %d\n", MapKey);

	return EFI_SUCCESS;
}
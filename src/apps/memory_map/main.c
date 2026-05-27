#include <efi.h>
#include <efilib.h>

// UINTN = Pointer-sized unsigned int

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);
	Print(L"MemoryMap Viewer Application!\n");
	Print(L"SystemTable pointer: 0x%lx\n", (UINTN)SystemTable);
	Print(L"Firmware Vendor: %s\n", SystemTable-> FirmwareVendor);
	Print(L"UEFI Revision: %d\n", SystemTable->Hdr.Revision);
	Print(L"BootServices: %p\n", SystemTable->BootServices);

	EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
	UINTN MemoryMapSize = 0;
	UINTN MapKey;
	UINTN DescriptorSize;
	UINT32 DescriptorVersion;
	EFI_STATUS Status;

	Status = SystemTable->BootServices->GetMemoryMap(
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

	Print(L"MemoryMapSize: %d\n", MemoryMapSize);

	Status = SystemTable->BootServices->AllocatePool(
		EfiLoaderData,
		MemoryMapSize,
		(void**)&MemoryMap
	);

	Status = SystemTable->BootServices->GetMemoryMap(
		&MemoryMapSize,
		MemoryMap,
		&MapKey,
		&DescriptorSize,
		&DescriptorVersion
	);

	Print(L"MemoryMapSize: %d\n", MemoryMapSize);

	for (
    	EFI_MEMORY_DESCRIPTOR *Desc = MemoryMap;
    	(UINT8*)Desc < (UINT8*)MemoryMap + MemoryMapSize;
    	Desc = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)Desc + DescriptorSize)
	) {
		Print(L"Type: %d Pages: %lu\n", Desc->Type, Desc->NumberOfPages);
	}

	UINTN Count = MemoryMapSize / DescriptorSize;

	Print(L"MemoryMapSize: %lx\n", MemoryMapSize);
	Print(L"DescriptorSize: %d\n", DescriptorSize);
	Print(L"MapKey: %d\n", MapKey);

	return EFI_SUCCESS;
}
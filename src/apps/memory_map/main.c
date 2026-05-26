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

	return EFI_SUCCESS;
}
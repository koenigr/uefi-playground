#include <efi.h>
#include <efilib.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);
	Print(L"Graphics Application!\n");

	EFI_STATUS Status;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;
	EFI_BOOT_SERVICES *BootServices = SystemTable->BootServices;

	Status = uefi_call_wrapper(
		BootServices->LocateProtocol,
		3,
		&gEfiGraphicsOutputProtocolGuid,
		NULL,
		(VOID**)&gop
	);

	if (EFI_ERROR(Status)) {
		Print(L"Failed to load Graphics Output Protocol: %r\n", Status);
		return Status;
	}

	return EFI_SUCCESS;
}
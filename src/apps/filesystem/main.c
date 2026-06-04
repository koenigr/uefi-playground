#include <efi.h>
#include <efilib.h>


EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);
	Print(L"Filesystem Application!\n");

	EFI_STATUS Status;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *Fs = NULL;
	EFI_BOOT_SERVICES *BootServices = SystemTable->BootServices;

	Status = uefi_call_wrapper(
		BootServices->LocateProtocol,
		3,
		&gEfiSimpleFileSystemProtocolGuid,
		NULL,
		(VOID**)&Fs
	);

	if (EFI_ERROR(Status)) {
		Print(L"Fehler beim Öffnen des Volumes: %r\n", Status);
		return Status;
	}

	Print(L"Located Simple File System Protocol\n");

	return EFI_SUCCESS;
}
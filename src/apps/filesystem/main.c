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
		Print(L"Failed to locate Simple File System Protocl: %r\n", Status);
		return Status;
	}

	Print(L"Located Simple File System Protocol\n");

	EFI_FILE_PROTOCOL *Root = NULL;

	Status = uefi_call_wrapper(
		Fs->OpenVolume,
		2,
		Fs,
		&Root
	);

	if (EFI_ERROR(Status)) {
		Print(L"Error while opening root volume: %r\n", Status);
		return Status;
	}

	Print(L"Opened root volume\n");

	return EFI_SUCCESS;
}
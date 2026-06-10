#include <efi.h>
#include <efilib.h>

EFI_STATUS OpenFile(EFI_FILE_PROTOCOL* Root, CHAR16* Path, EFI_FILE_PROTOCOL** File) {

	if (Root == NULL || Path == NULL || File == NULL)
		return EFI_INVALID_PARAMETER;

	EFI_STATUS Status;

	Status = uefi_call_wrapper(
		Root->Open,
		5,
		Root,
		File,
		Path,
		EFI_FILE_MODE_READ,
		0
	);

	return Status;
}


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

	VOID *Buffer = NULL;
	UINTN BufferSize = 1024;

	Status = uefi_call_wrapper(
		SystemTable->BootServices->AllocatePool,
		3,
		EfiLoaderData,
		BufferSize,
		&Buffer
	);

	if (EFI_ERROR(Status)) {
		Print(L"AllocatePool failed: %r\n", Status);
		return Status;
	}

	while(1) {

		BufferSize = 1024; 

		Status = uefi_call_wrapper(
			Root->Read,
			3,
			Root,
			&BufferSize,
			Buffer
		);

		if (EFI_ERROR(Status) && Status != EFI_BUFFER_TOO_SMALL) {
			Print(L"Read failed: %r\n", Status);
			return Status;
		}

		if (Status == EFI_BUFFER_TOO_SMALL) {

			FreePool(Buffer);
			Buffer = NULL;

			Status = uefi_call_wrapper(
				BootServices->AllocatePool,
				3,
				EfiLoaderData,
				BufferSize,
				&Buffer
			);

			if (EFI_ERROR(Status))
				return Status;

			Status = uefi_call_wrapper(
				Root->Read,
				3,
				Root,
				&BufferSize,
				Buffer
			);

			if (EFI_ERROR(Status)) {
				Print(L"Read failed: %r\n", Status);
				return Status;
			}

		}

		if (BufferSize == 0) {
			break;
		}

		EFI_FILE_INFO *FileInfo = (EFI_FILE_INFO*)Buffer;
		Print(L"%s\n", FileInfo->FileName);

		Print(L"    Size: %lu\n", FileInfo->FileSize);
		Print(L"    Physical Size: %lu\n", FileInfo->PhysicalSize);

		Print(L"    Attributes: ");

		if (FileInfo->Attribute & EFI_FILE_DIRECTORY)
			Print(L"Directory ");
		if (FileInfo->Attribute & EFI_FILE_ARCHIVE) 
			Print(L"Archive ");
		if (FileInfo->Attribute & EFI_FILE_READ_ONLY)
			Print(L"ReadOnly ");

		Print(L"\n\n");
	}

	EFI_FILE_PROTOCOL *File;

	Status = OpenFile(Root, L"test.txt", &File);

	if (!EFI_ERROR(Status)) {
		Print(L"Opened test.txt\n");

		UINTN InfoSize = 0;
		VOID *InfoBuffer = NULL;

		Status = uefi_call_wrapper(
			File->GetInfo,
			4,
			File,
			&gEfiFileInfoGuid,
			&InfoSize,
			InfoBuffer
		);


		if (Status == EFI_BUFFER_TOO_SMALL) {

			InfoBuffer = NULL;

			Status = uefi_call_wrapper(
				BootServices->AllocatePool,
				3,
				EfiLoaderData,
				InfoSize,
				&InfoBuffer
			);

			if (EFI_ERROR(Status)) {
				Print(L"AllocatePool failed: %r\n", Status);
				return Status;
			}

			Status = uefi_call_wrapper(
				File->GetInfo,
				4,
				File,
				&gEfiFileInfoGuid,
				&InfoSize,
				InfoBuffer
			);

			if (EFI_ERROR(Status)) {
				Print(L"GetInfo failed: %r\n", Status);
				return Status;
			}

			EFI_FILE_INFO *FileInfo = (EFI_FILE_INFO*)InfoBuffer;
			Print(L"File size: %lu\n", FileInfo->FileSize);

			VOID *FileBuffer = NULL;

			Status = uefi_call_wrapper(
				BootServices->AllocatePool,
				3,
				EfiLoaderData,
				FileInfo->FileSize + 1,
				&FileBuffer
			);

			if (EFI_ERROR(Status)) {
				Print(L"AllocatePool failed: %r\n", Status);
				return Status;
			}

			UINTN FileSize = FileInfo->FileSize;

			Status = uefi_call_wrapper(
				File->Read,
				3,
				File,
				&FileSize,
				FileBuffer
			);

			if (EFI_ERROR(Status)) {
				Print(L"Read failed: %r\n", Status);
				return Status;
			}

			((CHAR8*)FileBuffer)[FileSize] = '\0';

			CHAR8 *Text = (CHAR8*)FileBuffer;

			Print(L"\nContents:\n");

			for (UINTN i = 0; i < FileSize; i++) {
				Print(L"%c", Text[i]);
			}

			Print(L"\n");

			FreePool(FileBuffer);
			FreePool(InfoBuffer);

		}

		uefi_call_wrapper(
			File->Close,
			1,
			File
		);
		Print(L"Closed test.txt\n");
	}

	FreePool(Buffer);

	return EFI_SUCCESS;
}
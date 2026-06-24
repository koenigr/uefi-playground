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


EFI_STATUS GetFileSize(EFI_FILE_PROTOCOL* File, UINT64* Size, EFI_BOOT_SERVICES* BootServices) {
	if (File == NULL || Size == NULL) {
		return EFI_INVALID_PARAMETER;
	}


	UINTN InfoSize = 0;
	VOID *InfoBuffer = NULL;

	EFI_STATUS Status;

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

			uefi_call_wrapper(
				BootServices->FreePool,
				1,
				InfoBuffer
			);

			return Status;
		}

		EFI_FILE_INFO *FileInfo = (EFI_FILE_INFO*)InfoBuffer;
		*Size = FileInfo->FileSize;
	}

	uefi_call_wrapper(
		BootServices->FreePool,
		1,
		InfoBuffer
	);

	return EFI_SUCCESS;
}

EFI_STATUS ListDirectory(EFI_FILE_PROTOCOL *Dir, EFI_BOOT_SERVICES *BootServices, UINTN Depth) {

	if (Dir == NULL) {
		return EFI_INVALID_PARAMETER;
	}

	EFI_STATUS Status;
	UINTN BufferSize = 4096;
	VOID *Buffer = NULL;

	/*
	 * Make sure, that it always starts with position zero
	 */
/*	uefi_call_wrapper(
		Dir->SetPosition,
		2,
		Dir,
		0
	);*/

	/*
	 * Allocate Pool for EFI_FILE_INFO
	 */
	Status = uefi_call_wrapper(
		BootServices->AllocatePool,
		3,
		EfiLoaderData,
		BufferSize,
		&Buffer
	);

	if (EFI_ERROR(Status)) {
		Print(L"Failed to allocate Pool: %r\n", Status);
		return Status;
	}

	EFI_FILE_INFO *Entries[256];
	UINTN EntryCount = 0;

	while(1) {
		BufferSize = 4096;

		Status = uefi_call_wrapper(
			Dir->Read,
			3,
			Dir,
			&BufferSize,
			Buffer
		);

		if (EFI_ERROR(Status) && Status != EFI_BUFFER_TOO_SMALL) {
			uefi_call_wrapper(
				BootServices->FreePool,
				1,
				Buffer
			);

			return Status;
		}

		if (Status == EFI_BUFFER_TOO_SMALL) {
			uefi_call_wrapper(
				BootServices->FreePool,
				1,
				Buffer
			);


			Status = uefi_call_wrapper(
				BootServices->AllocatePool,
				3,
				EfiLoaderData,
				BufferSize,
				&Buffer
			);

			if (EFI_ERROR(Status)) {
				return Status;
			}

			Status = uefi_call_wrapper(
				Dir->Read,
				3,
				Dir,
				&BufferSize,
				Buffer
			);

			if (EFI_ERROR(Status)) {
				uefi_call_wrapper(
					BootServices->FreePool,
					1,
					Buffer
				);

				return Status;
			}

			continue;
		}

		if (BufferSize == 0) {
			break;
		}


		/*
		 * Read content of Buffer as EFI_FILE_INFO struct
		 */
		EFI_FILE_INFO *Info = (EFI_FILE_INFO *)Buffer;


		if (StrCmp(Info->FileName, L".") == 0 ||
			StrCmp(Info->FileName, L"..") == 0) {
			continue;
		}

		EFI_FILE_INFO *Copy = NULL;

		Status = uefi_call_wrapper(
			BootServices->AllocatePool,
			3, EfiLoaderData,
			BufferSize,
			(VOID**)&Copy
		);

		CopyMem(Copy, Info, BufferSize);

		Entries[EntryCount++] = Copy;

	}

	for (UINTN i = 0; i < EntryCount; i++) {
		EFI_FILE_INFO *Info = Entries[i];

		for (UINTN d = 0; d < Depth; d++) {
			Print(L"    ");
		}


		BOOLEAN IsLast = (i == EntryCount - 1);

		if (IsLast) {
			Print(L"└── ");
		} else {
			Print(L"├── ");
		}

		Print(L"%s\n", Info->FileName);

		if (Info->Attribute & EFI_FILE_DIRECTORY) {

			EFI_FILE_PROTOCOL *SubDir;

			Status = OpenFile(
				Dir,
				Info->FileName,
				&SubDir
			);

			if (!EFI_ERROR(Status)) {
				ListDirectory(
					SubDir,
					BootServices,
					Depth + 1
				);

				uefi_call_wrapper(
					SubDir->Close,
					1,
					SubDir
				);
			}
		}
	}

	for (UINTN i = 0; i < EntryCount; i++) {
		FreePool(Entries[i]);
	}

	uefi_call_wrapper(
		BootServices->FreePool,
		1,
		Buffer
	);

	return EFI_SUCCESS;

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
		Print(L"Failed to locate Simple File System Protocol: %r\n", Status);
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

	Print(L"Opened root volume\n\n");

	Status = ListDirectory(Root, BootServices, 0);

	if (EFI_ERROR(Status)) {
		Print(L"ListDirectory failed: %r\n", Status);
	}

	EFI_FILE_PROTOCOL *File;

	Status = OpenFile(Root, L"test.txt", &File);

	if (EFI_ERROR(Status)) {
		Print(L"OpenFile failed: %r\n", Status);
		return Status;
	}
	
	Print(L"\nOpened test.txt\n");

	UINT64 FileSize;

	Status = GetFileSize(File, &FileSize, BootServices);

	if (EFI_ERROR(Status)) {
		Print(L"GetFileSize failed: %r\n", Status);
		return Status;
	}

	VOID *FileBuffer = NULL;

	Status = uefi_call_wrapper(
		BootServices->AllocatePool,
		3,
		EfiLoaderData,
		FileSize + 1,
		&FileBuffer
	);

	if (EFI_ERROR(Status)) {
		Print(L"AllocatePool failed: %r\n", Status);
		return Status;
	}

	UINTN BytesToRead = (UINTN)FileSize;

	Status = uefi_call_wrapper(
		File->Read,
		3,
		File,
		&BytesToRead,
		FileBuffer
	);

	if (EFI_ERROR(Status)) {
		Print(L"Read failed: %r\n", Status);
		return Status;
	}

	((CHAR8*)FileBuffer)[FileSize] = '\0';

	CHAR8 *Text = (CHAR8*)FileBuffer;

	Print(L"Contents:\n");

	for (UINTN i = 0; i < FileSize; i++) {
		Print(L"%c", Text[i]);
	}

	Print(L"\n");

	uefi_call_wrapper(
		BootServices->FreePool,
		1,
		FileBuffer
	);


	uefi_call_wrapper(
		File->Close,
		1,
		File
	);

	Print(L"Closed test.txt\n");

	return EFI_SUCCESS;
}
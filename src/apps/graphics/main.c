#include <efi.h>
#include <efilib.h>

CHAR16 *PixelFormatToString(EFI_GRAPHICS_PIXEL_FORMAT PixelFormat) {
	switch(PixelFormat) {
	case PixelRedGreenBlueReserved8BitPerColor:
		return L"PixelRedGreenBlueReserved8BitPerColor";

	case PixelBlueGreenRedReserved8BitPerColor:
		return L"PixelBlueGreenRedReserved8BitPerColor";

	case PixelBitMask:
		return L"PixelBitMask";

	case PixelBltOnly:
		return L"PixelBltOnly";

	case PixelFormatMax:
		return L"PixelFormatMax";

	default:
		return L"Unknown";
	}
}

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

	EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode = gop->Mode;

	Print(L"Framebuffer Base: 0x%lx\n", Mode->FrameBufferBase);
	Print(L"Framebuffer Size: %lu bytes\n", Mode->FrameBufferSize);

	Print(L"Resolution		: %ux%u\n",
			Mode->Info->HorizontalResolution,
			Mode->Info->VerticalResolution
	);

	Print(L"Pixels/ScanLine	: %u\n",
			Mode->Info->PixelsPerScanLine
	);

	Print(L"Pixel Format	: %s\n",
			PixelFormatToString(Mode->Info->PixelFormat)
	);

	Print(L"Max Modes		: %u\n", Mode->MaxMode);
	Print(L"Current Mode: %u\n", Mode->Mode);

	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
	UINTN SizeOfInfo;

	for (UINT32 mode = 0; mode < Mode->MaxMode; mode++) {
		uefi_call_wrapper(
			gop->QueryMode,
			4,
			gop,
			mode,
			&SizeOfInfo,
			&Info
		);

		Print(
			L"Mode %u: %ux%u\n",
			mode,
			Info->HorizontalResolution,
			Info->VerticalResolution
		);

		uefi_call_wrapper(
			BootServices->FreePool,
			1,
			Info
		);
	}

	return EFI_SUCCESS;
}
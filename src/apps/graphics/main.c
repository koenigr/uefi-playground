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

void PrintAvailableModes(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_BOOT_SERVICES *BootServices) {

	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
	UINTN SizeOfInfo;

	EFI_STATUS Status;

	for (UINT32 mode = 0; mode < gop->Mode->MaxMode; mode++) {
		Status = uefi_call_wrapper(
			gop->QueryMode,
			4,
			gop,
			mode,
			&SizeOfInfo,
			&Info
		);

		if (EFI_ERROR(Status)) {
			continue;
		}

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
}

void DisplayCurrentModeInformation(EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode ) {
	Print(L"Max Modes		: %u\n", Mode->MaxMode);
	Print(L"Current Mode: %u\n", Mode->Mode);

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
}

void PutPixel(
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop,
	UINTN x,
	UINTN y,
	UINT32 color) {
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info = gop->Mode->Info;

	UINT32 *fb = (UINT32 *)gop->Mode->FrameBufferBase;

	UINTN offset = y * info->PixelsPerScanLine + x;

	fb[offset] = color;
}

void DrawHLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop,
	UINTN x, UINTN y, UINTN length, UINT32 color) {
	for (UINTN i = 0; i < length; i++) {
		PutPixel(gop, x + i, y, color);
	}
}

void DrawVLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop,
	UINTN x, UINTN y, UINTN length, UINT32 color) {
	for (UINTN i = 0; i < length; i++) {
		PutPixel(gop, x, y + i, color);
	}
}

void DrawRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop,
	UINTN x, UINTN y, UINTN width, UINTN height, UINT32 color) {
	DrawHLine(gop, x, y, width, color);
	DrawHLine(gop, x, y + height - 1, width, color);

	DrawVLine(gop, x, y, height, color);
	DrawVLine(gop, x + width - 1, y, height, color);
}

void FillRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop,
	UINTN x, UINTN y, UINTN width, UINTN height, UINT32 color) {
	for (UINTN j = 0; j < height; j++) {
		for (UINTN i = 0; i < width; i++) {
			PutPixel(gop, x + i, y + j, color);
		}
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

	DisplayCurrentModeInformation(Mode);

	// PrintAvailableModes(gop, BootServices);

#if 0 
	Status = uefi_call_wrapper(
		gop->SetMode,
		2,
		gop,
		0
	);

	if(EFI_ERROR(Status)) {
		Print(L"SetMode failed: %r\n", Status);
		return Status;
	}

	Mode = gop->Mode;

	DisplayCurrentModeInformation(Mode);
#endif

	PutPixel(gop, 10, 10, 0x00FF0000); // Red
	PutPixel(gop, 20, 10, 0x0000FF00); // Green
	PutPixel(gop, 30, 10, 0x000000FF); // Blue

	for (UINTN y = 100; y < 200; y++) {
		for (UINTN x = 100; x < 200; x++) {
			PutPixel(gop, x, y, 0x00FFFFFF);
		}
	}

	FillRect(gop, 50, 50, 200, 150, 0x0000FF00);

	DrawRect(gop, 300, 50, 200, 150, 0x00FF0000);

	DrawHLine(gop, 50, 250, 300, 0x00FFFFFF);
	DrawVLine(gop, 200, 300, 200, 0x00FFFFFF);

	return EFI_SUCCESS;
}
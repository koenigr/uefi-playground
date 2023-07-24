#!/bin/bash

mkdir -p hda-contents/EFI/boot

cp helloEfiApplication.efi hda-contents/
#cp memtest.efi hda-contents/EFI/boot/BOOT_X64.efi

qemu-system-x86_64 -bios OVMF.fd \
	-hda fat:rw:hda-contents -net none \
	-drive if=pflash,format=raw,readonly=on,file=OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=OVMF_VARS.fd

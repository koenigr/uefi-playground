ARCH            = $(shell uname -m | sed s,i[3456789]86,ia32,)

OBJS            = helloEfiApplication.o helloEfiBoot.o
TARGET          = helloEfiApplication.efi helloEfiBoot.efi

EFIINC          = /usr/include/efi
EFIINCS         = -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol
LIB             = /usr/lib64
EFILIB          = /usr/lib
EFI_CRT_OBJS    = $(EFILIB)/crt0-efi-$(ARCH).o
EFI_LDS         = $(EFILIB)/elf_$(ARCH)_efi.lds

CFLAGS          = $(EFIINCS) -fno-stack-protector -fpic \
		  -fshort-wchar -mno-red-zone -Wall 
ifeq ($(ARCH),x86_64)
  CFLAGS += -DEFI_FUNCTION_WRAPPER
endif

LDFLAGS         = -nostdlib -znocombreloc -T $(EFI_LDS) -shared \
		  -Bsymbolic -L $(EFILIB) -L $(LIB) $(EFI_CRT_OBJS) 

all: $(TARGET)

helloEfiApplication.so: $(OBJS)
	ld $(LDFLAGS) helloEfiApplication.o -o $@ -lefi -lgnuefi

helloEfiBoot.so: $(OBJS)
	ld $(LDFLAGS) helloEfiBoot.o -o $@ -lefi -lgnuefi

%.efi: %.so
	objcopy -j .text -j .sdata -j .data -j .dynamic \
		-j .dynsym  -j .rel -j .rela -j .reloc \
		--target=efi-app-$(ARCH) $^ $@

clean:
	rm -f *.o
	rm -f *.so
	rm -f *.efi
	rm -f hda-contents/*.efi

run: all
	mkdir -p hda-contents/EFI/boot

	cp helloEfiApplication.efi hda-contents/
	cp helloEfiBoot.efi hda-contents/
	#cp memtest.efi hda-contents/EFI/boot/BOOT_X64.efi

	qemu-system-x86_64 -bios OVMF.fd \
	-hda fat:rw:hda-contents -net none \
	-drive if=pflash,format=raw,readonly=on,file=OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=OVMF_VARS.fd
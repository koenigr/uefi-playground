ARCH            = $(shell uname -m | sed s,i[3456789]86,ia32,)

APP_OBJS        = build/helloEfiApplication.o
BOOT_OBJS				= build/helloEfiBoot.o
TARGET          = build/helloEfiApplication.efi build/helloEfiBoot.efi

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

OVMF_CODE				= /usr/share/OVMF/OVMF_CODE.fd
OVMF_VARS				= firmware/OVMF_VARS.fd



all: $(TARGET)

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/helloEfiApplication.so: $(APP_OBJS)
	ld $(LDFLAGS) $(APP_OBJS) -o $@ -lefi -lgnuefi

build/helloEfiBoot.so: $(BOOT_OBJS)
	ld $(LDFLAGS) $(BOOT_OBJS) -o $@ -lefi -lgnuefi

build/%.efi: build/%.so
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

	cp build/helloEfiApplication.efi hda-contents/
	cp build/helloEfiBoot.efi hda-contents/

	#cp helloEfiApplication.efi hda-contents/EFI/boot/BOOT_X64.efi

	qemu-system-x86_64 -bios OVMF.fd \
	-hda fat:rw:hda-contents -net none \
	-drive if=pflash,format=raw,readonly=on,file=$(OVMF_CODE) \
	-drive if=pflash,format=raw,file=$(OVMF_VARS)
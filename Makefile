ARCH            = $(shell uname -m | sed s,i[3456789]86,ia32,)

APPS = hello memory_map graphics filesystem
BOOT = template linux_loader

TARGETS					= \
			$(addprefix build/apps/, $(addsuffix .efi, $(APPS))) \
			$(addprefix build/boot/, $(addsuffix .efi, $(BOOT)))

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


all: $(TARGETS)

build/apps/%.o: src/apps/%/main.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/boot/%.o: src/boot/%/main.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/%.so: build/%.o
	ld $(LDFLAGS) $^ -o $@ -lefi -lgnuefi

build/%.efi: build/%.so
	objcopy -j .text -j .sdata -j .data -j .dynamic \
		-j .dynsym  -j .rel -j .rela -j .reloc \
		--target=efi-app-$(ARCH) $< $@

clean:
	rm -rf build
	rm -f hda-contents/*.efi

run: all
	mkdir -p hda-contents/EFI/boot

	cp build/apps/*.efi hda-contents/
	cp build/boot/*.efi hda-contents/

	#cp helloEfiApplication.efi hda-contents/EFI/boot/BOOT_X64.efi

	qemu-system-x86_64 -bios OVMF.fd \
	-hda fat:rw:hda-contents -net none -serial stdio \
	-drive if=pflash,format=raw,readonly=on,file=$(OVMF_CODE) \
	-drive if=pflash,format=raw,file=$(OVMF_VARS)
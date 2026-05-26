# Uefi-Playground

Minimal UEFI application written in C,
bootable in QEMU using OVMF firmware.

This project explores:
- UEFI applications
- low-level boot environments
- firmware interfaces
- QEMU-based system emulation

Main goal is to see differences between a simple Application inside UEFI and a bootable Executable.

# Explanation Difference between Application and Bootable

# Screenshot
Bootscreen in QEMU

![UEFI Boot in QEMU showing HelloEfi output](assets/Running.jpg)


# Architektur
Host Linux  
	|  
Build EFI Binary  
	|  
QEMU + OVMF  
	|  
UEFI Application  

# Dependencies

Ubuntu/Debian:
```bash
sudo apt install ovmf qemu-system-x86 gnu-efi 
```

# Build Instructions
```bash
make run
```

# UEFI Interactive Shell - commands
Switch HD: ```fs0:```
List files: ```ls```
To leave shell: ```exit```


# Applications
## Hello
Just a template how to create an application


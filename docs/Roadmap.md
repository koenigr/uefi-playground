Meine eigenen Pläne:

HINTS:
If you are using gnu-efi, use uefi_call_wrapper() to call UEFI functions.

Check the bootboot project

TODO:
Analyze difference between helloEfiApplication and helloEfiBoot

Analyze difference between helloEfiBoot and MemtestBoot

How to get from helloEfiBoot to helloEfiApplication?

How to get from MemtestBoot to MemtestApplication?


UEFI App
  ↓ setzt boot flag
Linux boot
  ↓ liest flag
Kernel module autoloaded

Debugging mit GDB über QEMU machen




Dockerfile oder Nix oder fertige Build-Scripts
./build.sh
./run_qemu.sh



GitHub Actions:

buildet das EFI binary
startet evtl. QEMU headless
prüft ob binary erzeugt wurde



Idee A — Tiny UEFI Bootloader


Features
eigener Bootscreen
EFI memory map anzeigen
Kernel laden
ELF parser
framebuffer initialisieren
in Long Mode springen




Idee B — UEFI + eigener Mini-Kernel

Das wäre mein Favorit.

Architektur
UEFI App
   ↓
lädt Kernel
   ↓
springt in Kernel
   ↓
Kernel schreibt auf Framebuffer



Firmware
Bootloader
Kernel
Memory Management
ELF Loading
Low-Level C



Idee C — Hardware/Embedded Simulation



Beispiel

UEFI-App simuliert:

UART
Timer
GPIO Register

Oder:

kleines MMIO Demo
fake sensor registers

Dann:

„Embedded concepts without hardware“



Idee D — Linux Kernel Modul als zweites Repo

NICHT direkt integrieren.

Lieber:

Repo 1

HelloEfi
→ Bootprozess / Firmware

Repo 2

Linux Kernel Module
→ Treiber / Kernel APIs

Das wirkt strukturierter.

Ideen für ein Kernel-Modul
Einfach aber beeindruckend
1. Character Device Driver
/dev/helloefi

User-space ↔ Kernel-space Kommunikation.



2. Procfs/Sysfs Modul

Expose:

CPU info
memory stats
fake sensor



3. Mini GPIO Driver (simuliert)

Auch ohne echte Hardware.


Projektname

Nicht „HelloEfi“.

Besser:

tiny-uefi-loader
qboot
ember-efi
microboot
uefi-playground
bootlab
Zielversion
Features

Boots in QEMU
eigener UEFI Bootscreen
Memory Map anzeigen
ELF Kernel laden
Framebuffer Output
CI Build
Docker Build Environment
gute Dokumentation
Architekturdiagramm

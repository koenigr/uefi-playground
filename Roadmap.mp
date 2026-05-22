Meine eigenen Pläne:

HINTS:
If you are using gnu-efi, use uefi_call_wrapper() to call UEFI functions.

Check the bootboot project

TODO:
Analyze difference between helloEfiApplication and helloEfiBoot

Analyze difference between helloEfiBoot and MemtestBoot

How to get from helloEfiBoot to helloEfiApplication?

How to get from MemtestBoot to MemtestApplication?







Was an dem Projekt schon gut ist

UEFI bedeutet:

Bootprozess
Bare-Metal-Nähe
kein Betriebssystem
Speicherlayout
Toolchains
Cross-Compilation
Emulatoren/QEMU
Firmware Interfaces
evtl. PE/COFF-Binaries
Debugging ohne Komfort

Das schreit eigentlich schon:

„Ich interessiere mich ernsthaft für Systems Programming.“

Das ist wertvoll für:

Embedded
Firmware
BSP/Bootloader
Automotive
Linux Kernel
RTOS
Hypervisoren
Security/Reverse Engineering
Mein Vorschlag: Projekt modernisieren + aufwerten

Nicht riesig machen.
Lieber:

sauber,
dokumentiert,
technisch interessant,
gut erklärbar.

Das beeindruckt mehr.

Phase 1 — Aufräumen

Ziel:

„Das Projekt wirkt professionell.“

Dinge, die du vermutlich machen solltest
1. Struktur modernisieren

Typisch gut:

HelloEfi/
├── README.md
├── Makefile
├── docker/
├── scripts/
├── src/
│   └── main.c
├── include/
├── build/
├── run_qemu.sh
├── .github/workflows/
└── docs/
2. README massiv verbessern

Das ist bei Portfolio-Projekten extrem wichtig.

README sollte enthalten:

Was ist das Projekt?
Warum ist es interessant?
Architekturdiagramm
Build-Anleitung
QEMU-Setup
Screenshots/GIFs
Technische Learnings
Roadmap
3. Build reproduzierbar machen

SEHR wichtig.

Ideal:

Dockerfile
oder Nix
oder fertige Build-Scripts

Dann kann jeder schreiben:

./build.sh
./run_qemu.sh

Das wirkt sofort professionell.

4. CI/CD hinzufügen

GitHub Actions:

buildet das EFI binary
startet evtl. QEMU headless
prüft ob binary erzeugt wurde

Für Embedded extrem stark im Portfolio.

Phase 2 — Technisch interessanter machen

Jetzt die spannenden Ideen.

Idee A — Tiny UEFI Bootloader

Sehr stark für Portfolio.

Features
eigener Bootscreen
EFI memory map anzeigen
Kernel laden
ELF parser
framebuffer initialisieren
in Long Mode springen

Das ist schon fast „mini OS dev“.

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

Dann hast du:

Firmware
Bootloader
Kernel
Memory Management
ELF Loading
Low-Level C

Das ist GOLD für Embedded/System Programming.

Idee C — Hardware/Embedded Simulation

Da du Embedded machen willst:

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

Das verstehen Recruiter sofort.

Idee D — Linux Kernel Modul als zweites Repo

Das würde ich NICHT direkt integrieren.

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

Sehr gutes Lernprojekt.

2. Procfs/Sysfs Modul

Expose:

CPU info
memory stats
fake sensor

Sehr embedded-nah.

3. Mini GPIO Driver (simuliert)

Auch ohne echte Hardware.

Was ich persönlich machen würde
Portfolio-Version
Projektname

Nicht „HelloEfi“.

Das klingt nach Tutorial.

Besser:

tiny-uefi-loader
qboot
ember-efi
microboot
uefi-playground
bootlab
Zielversion
Features

✅ Boots in QEMU
✅ eigener UEFI Bootscreen
✅ Memory Map anzeigen
✅ ELF Kernel laden
✅ Framebuffer Output
✅ CI Build
✅ Docker Build Environment
✅ gute Dokumentation
✅ Architekturdiagramm

Das reicht komplett.

Was Recruiter daran mögen würden

Du demonstrierst:

Low-Level C
Buildsystems
Toolchains
Emulatoren
Linux
Debugging
Firmware
Architekturverständnis
Selbstständigkeit

Das ist deutlich stärker als:

„Todo App mit React“

Nächster sinnvoller Schritt

Schick mir am besten entweder:

den HTTPS-GitHub-Link
(nicht SSH, weil ich den nicht öffnen kann)

oder

ZIP / einzelne Dateien hier

Dann kann ich konkret helfen bei:

Code-Review
Projektstruktur
README
Buildsystem
Architektur
Roadmap
„Was lohnt sich wirklich?“
und einem realistischen Plan für ein starkes Embedded-Portfolioprojekt.

Außerdem kann ich dir dann sagen:

„Das hier behalten.“
„Das hier wegwerfen.“
„Das hier modernisieren.“
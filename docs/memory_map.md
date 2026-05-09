# Memory Map

The emulator exposes a 64 KB address space.

| Address range | Purpose |
| --- | --- |
| 0x0000-0x1FFF | Program ROM area loaded from assembled binaries |
| 0x2000-0xDFFF | General RAM and stack space |
| 0xE000-0xE0FF | Memory mapped IO |
| 0xE100-0xFFEF | Reserved or extra RAM |
| 0xFFF0-0xFFFE | Future interrupt vectors |
| 0xFFFF | Reserved |

## IO Registers

| Address | Name | Behavior |
| --- | --- | --- |
| 0xE000 | Console character output | Writes one ASCII character |
| 0xE001 | Console number output | Writes the unsigned decimal value and a space |
| 0xE002 | Timer low byte | Stores and displays the timer value |
| 0xE003 | Timer high byte | Stores the high timer byte |
| 0xE004 | Status | Generic status register |

The stack starts at `0xDFFF` and grows downward.

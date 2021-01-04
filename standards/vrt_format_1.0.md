## VRT - Vic20 caRTridge Image (for the VICE xvic emulator)

Document revision 0.0

Last updated: Jan 4, 2021

Compiler/Editor: Aubrey McLeod

Contributors/sources: Peter Schepers,
                         Per Hakan Sundell,
                         Markus Brenner,
                         Marco Van Den Heuvel,
                         Tobias
                         
The .CRT files was introduced in the CCS64 emulator, written by Per Hakan Sundell.

This variant, designed for the xVIC emulator, derives from the 1.13 revision of the specification released on June 12, 2007; it uses the ".VRT" file extension, and attempts to maintain the overall structure of the ".CRT" file type.

### Header Block

| component               | addresses  | size    | contents | note |
|-------------------------|------------|---------|----------|------|
| Cartridge Signature     | $0000-000F | 16 bytes| "VIC20 CARTRIDGE " | ascii encoded and padded with space |
| File header length      | $0010-0013 | 4 bytes | The length of the header block in bytes, from offset $0000 | encoded in high/low format; minimum 0x40 |
| Cartridge version       | $0014-0015 | 2 bytes | The cartridge version | high/low, presently 02.00 |
| Cartridge hardware type | $0016-0017 | 2 bytes | One of the following values: | high/low|
|                         |            |         | 0                            | Generic cart (default) |
|                         |            |         | 1                            | Megacart               |
|                         |            |         | 2                            | Final Expansion        |
|                         |            |         | 3                            | Vic Flash Plugin       |
|                         |            |         | 4                            | IEEE-4888              |
|                         |            |         | 5                            | SIDCart                |
|                         |            |         | 6                            | Ultimem                |
|                         |            |         | 7                            | IO ram cart (IO2)      |
|                         |            |         | 8                            | IO ram cart (IO3)      |
|                         |            |         | 9                            | BehrBonz               |
| | | | |
| Reserved for future use | $0018-0019     | 2 bytes  | | the .CRT format from which .VRT is derived uses these bytes for cartridge port memory remapping (PLA). Because VIC20 doesn't have said functionality, the space has been left reserved for future use. |
| Reserved for future use | $001A-001C | 3 bytes | | |
| Video mode flags        | $001D      | 1 byte  | contains a series of bit flags which specify video mode information, more detail on specific values below | |
|                         |            |         | 0b00000000 | no video mode is specifically required; NTSC is preferred |
|                         |            |         | 0b00000001 | no video mode is specifically required; PAL is preferred |
|                         |            |         | 0b00000010 | the cartridge requires NTSC mode to work correctly |
|                         |            |         | 0b00000011 | the cartridge requires PAL mode to work correctly |
| | | | |
| ROM start address       | $001E-001F | 2 bytes | contains the default location that an application starts | defaults to $A000; high/low encoding |
| cartridge name          | $0020-003F | 32 btes | Contains the name of the cartridge | uppercase and passed with null characters |
| cartridge contents (CHIP packets) | $0040-xxxx | see below for a breakdown of the CHIP format| |


### CHIP contents

Offset by $40 bytes, so here $0000 = $0040.

| component               | addresses  | size    | contents | note |
|-------------------------|------------|---------|----------|------|
| 'CHIP' signature        | $0000-0003 | 4 bytes | "CHIP"   | there can be more than one image in a .VRT file |
| Total packet length     | $0004-0007 | 4 bytes | Total length of ROM image size and header combined | High/Low format |
| Chip type               | $0008-0009 | 2 bytes | One of the following options: |  High/Low format |
|                         |            |         | 0                             | ROM              |
|                         |            |         | 1                             | RAM, no ROM data |
|                         |            |         | 2                             | Flash ROM        |
| | | | |
| Bank Number             | $000A-000B | 2 bytes | Defaults to $0000 for normal carts | High/Low format |
| Starting load address   | $000C-000D | 2 bytes | The address, typically block aligned where a given chip is loaded | High/Low format |
| ROM image size          | $000E-000F | 2 bytes | the size of the ROM image in bytes | High low format, typically $2000 or $4000 |
| ROM data                | $0010-xxxx | The raw ROM dump | |

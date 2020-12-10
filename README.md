# Vic-20 ROM format

### Preamble:
I find it terribly frustrating that there currently exists no good format for loading and storing VIC-20 ROMS.
Initially, I realized this was a problem when tinkering with a Raspberry Pi build of VICE; the fact that some 
cartridge ROMs are stored in multiple files made the process of loading games with a controller very inconvenient.
  
While, I originally got around this by writing a simple script to load the files via VICE's CLI, I would rather 
build something more stable, with slightly more longevity.

Accordingly, I intend to devise a standard for VIC-20 ROM files (detailed in the V20v1.md file), and create a command line utility
for converting existing ROMs into the new .v20 format.
  
In the long term, I might create a UI based tool,
and I might even go so far as to patch VICE to accept these .v20 files; the key thing here is getting the ball rolling.

### The makev20 utility:
The current specification uses a tool called `makev20`. The tool takes (multiple) existing `.crt`, `.prg`, and miscellaneous binaries, and attempts to convert them into our eponymous `.v20` format. Some binaries have useful `2 byte` addressing information embedded into them, `makev20` will attempt to automatically assign these binaries to the correct address. Unfortunately, you will have to tell `makev20` where binaries that lack this information are meant to live.



`makev20` itself is written in C, and attempts to be relatively portable in its approach. To actually use it on your system of choice, either download one of the precompiled binaries, or feel free to compile it yourself. It is a command line tool, and I have attempted to make it flexible and intuitive.


The `makev20` utility has the following arguments:

| name | argument | example | description | note|
|------|----------|---------|-------------|-----|
| `input_flag` |`-i [destination_flag] <path_to_ROM> [[destination_flag] <path_to_additional_ROM(s)>...]` | `makev20 -i <some_input_path> [some_other_input_path] -o <some_output_path>` | The `-i` flag denotes input files and is immediately followed by the space-separated list of paths to ROM files that you are using. | requires a corresponding `-o` flag to convert ROMs. |
| `output_flag` |`-o <path_to_output>` | `makev20 -i <some_input_path> -o <some_output_path>` | The `-o` flag denotes your output destination, it is immediately followed by a path to the exported file. | requires a corresponding `-i` flag to convert ROMs.|
| `overwrite_flag` |`[-ow]` | `makev20 -i <some_input_path> -o <some_output_path> -ow` | The `-ow` flag forces `makev20` to write a new file to the specified location, even if it overwrites an existing file. | can only be performed on conversion operations. |
| `ROM_name_flag` | `[-n <"ROM_NAME">]` | `makev20 -i <some_input_path> -o <some_output_path> -n <"ROM_NAME">` | The `-n` flag allows you to specify a name for the ROM. | At most 32 ASCII characters (anything more will be truncated). |
| `Auto-Exec_flag` | `[-ae <"COMMAND">]` | `makev20 -i <some_input_path> -o <some_output_path> -ae "<COMMAND>"` | The `-ae` flag allows you to specify a command to be executed on boot; this is very useful for games like the Scott Adam's Adventure series. | At most 22 ASCII characters long (will be truncated). |
| `video_type_flag` | `[-v <ntsc/pal>]` | `makev20 -i <some_input_path> -o <some_output_path> -v pal` | While most ROMs seem fairly platform agnostic, there are a few that rely on the different timings, it makes sense then, to allow a ROM to track its own video mode. | defaults to ntsc if not set. |
| | | | | |
| | | | | |
| `$A000_memory_address` |`--a0` | `-i --a0 <path_to_ROM> [some_other_path]` | The `--a0` destination flag causes the input source file to be loaded at the memory location `$A000` | `$A000` is the auto-load location. |
| `$B000_memory_address` |`--b0` | `-i --b0 <path_to_ROM> [some_other_path]` | The `--b0` destination flag causes the input source file to be loaded at the memory location `$B000` | `$B000` can only fit a `4kb` block.|
| `$2000_memory_address` |`--20` | `-i --20 <path_to_ROM> [some_other_path]` | The `--20` destination flag causes the input source file to be loaded at the memory location `$2000` | |
| `$4000_memory_address` |`--40` | `-i --40 <path_to_ROM> [some_other_path]` | The `--40` destination flag causes the input source file to be loaded at the memory location `$4000` | |
| `$6000_memory_address` |`--60` | `-i --60 <path_to_ROM> [some_other_path]` | The `--60` destination flag causes the input source file to be loaded at the memory location `$6000` | |
| | | | | |
| | | | | |
| `version_flag` | `-version` | `makev20 -version` | The `-version` flag displays the version number of the current installation. | |

Please note that `makev20` attempts to manage the address-space, so a `16kb` ROM sent to `$4000-$5FFF` will fill bank `$6000-$7FFF` as well, however a block sent to `$6000-$7FFF` will take precedence and overwrite the original large block in that region.

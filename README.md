# Vic20 ROM format
The `.v20 format` is a simple format for Vic-20 cartridge ROMs, which attempts to streamline the existing multi-file hell of emulating the Vic-20. This repo defines both a standard for the file format, and offers a utility (`makev20`) for converting existing ROMs into a more manageable format. Related projects include a wrapper (to be released) which loads `.v20 files` into the popular Vice emulator, a fork of Vice (to be released), and a GUI tool(to be released) for making this process just a little bit more accessable. 

`makev20` itself is written in C, and strives to be portable. While I cannot provide a prebuilt version for every single operating system/architecture, I can at least provide sourcecode so that you can build the utility on your target platform. 
  
This project is open source so feel free to contribute or even make your own radical alterations. My only ask is that we try not to fragment the ecosystem, collaborate on standard version rather than making a wholley proprietary new varient.

### The Standard
Any and all official releases of the `.v20` standard are defined in the `.standards/` directory; you can find documentation the ROM format there. The current release is version `01`, but there may very well be newer standards in the future if the community needs more functionality out of the `.v20` format.

Ideally, new standards will not alter`.v20` too radically and ephasize maintaining backwards compatability, however all standards will include a version number in their header to help prevent undesired breaking.


### The makev20 utility:
The `makev20` is a utility for converting existing ROMs (`.bin`, `.prg`, `.crt`, `.20`, `.40`, `.60`, `.a0`, `.b0`, etc) into the .v20 format. The tool takes (multiple) existing ROMs as inputs and outputs new single `.v20` file derived from its inputs. Many binaries are encoded with a `2 byte` addressing information embedded into them; if this is present `makev20` will attempt to assign these binaries to their listed addresses. Unfortunately, for ROMs that lack this informaton, you will need to tell `makev20` where these binaries are meant to live.

It must be noted that `make20` is a command line utility, and on some levels is inherently not terribly user friendly. That being said, I have attempted to make it flexible and intuitive.

The `makev20` utility has the following arguments:

| name | argument | example | description | note|
|------|----------|---------|-------------|-----|
| `input_flag` |`-i [destination_flag] "<path_to_ROM>" [[destination_flag] <path_to_additional_ROM(s)>...]` | `makev20 -i <some_input_path> [some_other_input_path] -o <some_output_path>` | The `-i` flag denotes input files and is immediately followed by the space-separated list of paths to ROM files that you are using. | requires a corresponding `-o` flag to convert ROMs. |
| `output_flag` |`-o <path_to_output>` | `makev20 -i "<some_input_path>" -o "<some_output_path>"` | The `-o` flag denotes your output destination, it is immediately followed by a path to the exported file. | requires a corresponding `-i` flag to convert ROMs.|
| `overwrite_flag` |`[-ow]` | `makev20 -i "<some_input_path>" -o "<some_output_path>" -ow` | The `-ow` flag forces `makev20` to write a new file to the specified location, even if it overwrites an existing file. | can only be performed on conversion operations. |
| `ROM_name_flag` | `[-n <"ROM_NAME">]` | `makev20 -i "<some_input_path>" -o <some_output_path> -n "<"ROM_NAME">"` | The `-n` flag allows you to specify a name for the ROM. | At most 32 ASCII characters (anything more will be truncated). |
| `Auto_Exec_flag` | `[-ae <"COMMAND">]` | `makev20 -i "<some_input_path>" -o "<some_output_path>" -ae "<COMMAND>"` | The `-ae` flag allows you to specify a command to be executed on boot; this is very useful for games like the Scott Adam's Adventure series. | At most 22 ASCII characters long (will be truncated). |
| `video_type_flag` | `[-v <ntsc/pal>]` | `makev20 -i "<some_input_path>" -o "<some_output_path>" -v pal` | While most ROMs seem fairly platform agnostic, there are a few that rely on the different timings, it makes sense then, to allow a ROM to track its own video mode. | defaults to ntsc if not set. |
| | | | | |
| | | | | |
| `$A000_memory_address` |`--a0` | `-i --a0 "<path_to_ROM>" [some_other_path]` | The `--a0` destination flag causes the input source file to be loaded at the memory location `$A000` | `$A000` is the auto-load location. |
| `$B000_memory_address` |`--b0` | `-i --b0 "<path_to_ROM>" [some_other_path]` | The `--b0` destination flag causes the input source file to be loaded at the memory location `$B000` | `$B000` can only fit a `4kb` block.|
| `$2000_memory_address` |`--20` | `-i --20 "<path_to_ROM>" [some_other_path]` | The `--20` destination flag causes the input source file to be loaded at the memory location `$2000` | |
| `$4000_memory_address` |`--40` | `-i --40 "<path_to_ROM>" [some_other_path]` | The `--40` destination flag causes the input source file to be loaded at the memory location `$4000` | |
| `$6000_memory_address` |`--60` | `-i --60 "<path_to_ROM>" [some_other_path]` | The `--60` destination flag causes the input source file to be loaded at the memory location `$6000` | |
| | | | | |
| | | | | |
| `version_flag` | `-version` | `makev20 -version` | The `-version` flag displays the version number of the current installation. | |
| `help_flag` | `-help` | `makev20 -help` | The `-help` flag displays a list of all command flags. | |
| `debug_flag` | `-db` | `-i <path_to_ROM> -o <path_to_output> -db` | The `-db` flag outputs a fairly friendly readout of what the parser actually read and assigned based on your arguments |

Please note that `makev20` attempts to manage the address-space, so a `16kb` ROM sent to `$4000-$5FFF` will fill bank `$6000-$7FFF` as well, however a block sent to `$6000-$7FFF` will take precedence and overwrite the original large block in that region.


### Project Goals
- [X] Finalize `.v20-1` standard.
- [X] Implement minimum functionality to convert existing ROMs to the `.v20-1` format; `makev20` utility.
- [ ] Document code.
- [ ] Rewrite messy source-files.
- [X] Clean-up `.md` documentation.
- [ ] Write Vice-wrapper (done in a separate repo).
- [ ] Fork Vice and implement native `.v20` support (done in a separate repo).
- [ ] Create optional GUI frontend for `makev20` (done in a separate repo).
- [ ] Examine potential updates to standards
- [ ] TBD
  
### Why this exists? (by Aubrey McLeod)
If you are anything like me, you love classic computing platforms like Commodore's PET, VIC-20, C64, etc. In spite of their limitations, or perhaps even because of them, there is something indescribably magical about these machines; in a sense, they tipafy the eras' of their releases. While the C64 was by far the most successful of Commodore's offerings, the VIC-20 presented a compelling low cost option and introduced countless children to computing. Being the first computer model to sell 1,000,000 units, this little bread-binned beauty was key to Jack Tramiel's strategy for undercutting the competition, and arguably paved the way the much lauded C64.

Surprisingly, I only got started with Commodore machine's in the mid 2010s when I spent a summer restoring my grandparents old PET 4032 (good ol' fat 40). After that, I managed to get a couple of VIC-20s on the cheap and had a great deal of fun playing both cartridge and tape games, while trying to code within the 3.5k limits. Saddly, faulty old power supplies have long since killed my dear VIC-20s, but I still enjoyed my time with them. 

Thankfully, the advent of low-cost/low-power ARM based computers has made it easier than ever to recreate the magic of these old micros (here's lookin at you, Raspberry Pi 400). When I first set up a Vice install on my Pi 4, I discovered the problematic nature of Vic-20 cartridge ROMs; there existed no convenient way to load most cartridges with a single action. To the best of my knowledge, there exists no single ROM format that is sufficiently complex enough to load an entire game in to the Vic's split memory map, at least at time of writing (Dec 2020). While the pseudo-standard of including the 2 byte address before the ROM data is widely used, many games are still split between two separate files. In the context of my Raspberry Pi 4 running RetroPie, I initially hacked together a [wrapper written in Bash](https://retropie.org.uk/forum/topic/26186/tutorial-how-to-load-vic-20-cartridges-straight-from-emulationstation?_=1608355854478) to automatically load archived ROM files with specific name-flags into Vice. Unfortunately, this always felt a little bit slap-dash, and dirty. As a personal project during my third-year exams, I decided that the perfect destresser would be to impose some degree of order onto the Vic-20 landscape: design a new format which the community clearly needs. 

One of the big problems with tackling a problem some 40 years in the making, is that the web is full of prexisting ROMs which, more-or-less, work; no-one is going to use a new format as their baseline. It made sense to me, then, to make a utility which would allow all/most of the existing ROMs to be converted fairly easily. The other big problem is of course that no existing emulator supports a new format out of the gate. After finalizing a version of the `v20` standard/conversion utility, I hope to address this particular process in two steps:
1. creating a script wrapper to load `.v20 files` into existing builds of Vice through command line arguments.
2. fork Vice, patch in native support for `.v20 files`, and hope that given time the Vice team will see the merits of my format.

The wrapper is a bit of a stop-gap, and probalby won't be able to do everything I would like to with this format. I do not believe that Vice supports `basic` instruction injection directly through the command line; there may be a work arounds based on loading a tape/disk image as well, but I need to experiment.

In the long term, I hope to create a GUI based variant of this utility. It will not of course, be as portable as the CLI version but I would try to target all major platforms: Windows, Linux, and Mac. 

tl;dr I wanted to fix an old problem, that was bothering me.

### Related project
- GUI Tool (coming soon)
- Wrapper Scripts (coming soon)
- Patched Vice (coming soon)

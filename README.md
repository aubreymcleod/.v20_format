# .V20-generator
A utility to convert VIC-20 ROMs into a standard format.

I find it terribly frustrating that there currently exists no good format for loading and storing VIC-20 ROMS.
Initially, I realized this was a problem when tinkering with a Raspberry Pi build of VICE; the fact that some 
cartridge ROMs are stored in multiple files made the process of loading games with a controller very inconvenient.
  
While, I originally got around this by writing a simple script to load the files via VICE's CLI, I would rather 
build something more stable, with slightly more longevity.

Accordingly, I intend to set out a standard for VIC-20 ROM files (detailed in the V20v1.md file), and create a command line tool
to convert existing ROMs into the new .V20 format.
  
In the long term, I might fork off this project and create a UI variant,
and I might even go so far as to patch VICE to accept these .V20 files; the key thing here is getting the ball rolling.
# gameboy.cpp
## Table of Contents
- [gameboy_create](#gameboy_create)
- [gameboy_free](#gameboy_free)
- [gameboy_load](#gameboy_load)
## Functions
### gameboy_create
`() -> GameBoy *`  
Creates a GameBoy reference. This reference will be passed to very subsequent DLL call as an identifier. The reference **has** to be freed with [gameboy_free](#gameboy_free) once it is no longer needed.
### gameboy_load
`(GameBoy *gb, const char *bios, const char *rom, const char *sav) -> int`  
Boots up the emulator. Passing a path to valid BIOS and ROM images is required, where as  
SAV data is optional. SAV data is hot-loaded into the emulator as opposed to an OS-level file copy, so it is recommended to not have a physical .sav file accompany the ROM file in case emulation is needed to start from a fresh save.  
This function will return 0 on success and a negative value on failure. The error codes are defined [here](https://github.com/pokemon-speedrunning/gambatte-core/blob/master/libgambatte/include/loadres.h).
### gameboy_free
`(GameBoy *gb) -> void`  
Frees the GameBoy reference and any internal data assosicated with it.

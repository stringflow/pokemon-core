# grid-overlay.cpp
## Table of Contents
- [gameboy_hidegrid](#gameboy_hidegrid)
- [gameboy_showgrid](#gameboy_showgrid)
## Functions
### gameboy_hidegrid
`(GameBoy *gb) -> void`  
Hides the grid overlay.
### gameboy_showgrid
`(GameBoy *gb) -> void`  
Displays a grid overlay showing the path that the player is taking, useful for the creation of videos displaying a manip path. For this functionality to work properly, you must use gameboy_executepath for overworld movement.

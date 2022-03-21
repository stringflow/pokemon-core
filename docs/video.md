# video.cpp
## Table of Contents
- [gameboy_screenshot](#gameboy_screenshot)
- [gameboy_setvideocallback](#gameboy_setvideocallback)
- [gameboy_startrecording](#gameboy_startrecording)
- [gameboy_stoprecording](#gameboy_stoprecording)
## Functions
### gameboy_startrecording
`(GameBoy *gb, int width, int height) -> void`  
Starts recording the emulator's video. Requires [ffmpeg](https://www.ffmpeg.org/) to be located in PATH to function properly. The recording dimensions must at least be 160x144, but may be larger causing upscaling of the video. Video and audio will be recorded seperately, calling [gameboy_stoprecording](#gameboy_stoprecording) will merge the two into a final file.
### gameboy_stoprecording
`(GameBoy *gb, const char *video_title) -> void`  
Stops the recording and merges the video and audio files into one final output saved to the specified file path. The video title must include the file extension.
### gameboy_screenshot
`(GameBoy *gb, const char *filename) -> bool`  
Saves a screenshot of the emulator's current video to the specified file. The only two codecs currently supported are `.bmp` and `.png`. Returns whether or not the file was saved correctly.
### gameboy_setvideocallback
`(GameBoy *gb, VideoCallback callback) -> void`  
Sets a callback for whenever a new video frame is ready to be displayed.  
The callback will have access to the video buffer (R/W). This callback will be called **after** the optional grid overlay has already been drawn to the video buffer.

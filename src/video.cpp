#include "codec-bmp.h"
#include "codec-png.h"

constexpr const char *TEMP_VIDEO = "video.mp4";
constexpr const char *TEMP_AUDIO = "audio.mp4";

#ifdef COMPILER_MSVC
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif

FFMPEGPipe * ffmpeg_open_pipe(int width, int height, u64 start_sample_count) {
    FFMPEGPipe *pipe = new FFMPEGPipe();
    char command[256];
    sprintf(command, 
            "ffmpeg -y -f rawvideo -s %dx%d -pix_fmt bgra -r 60 -i - -crf 0 -vf scale=%d:%d:flags=neighbor %s", VIDEO_WIDTH, VIDEO_HEIGHT, width, height, TEMP_VIDEO);
    pipe->video = POPEN(command, "wb");
    sprintf(command, 
            "ffmpeg -y -f s16le -ar 2097152 -ac 2 -i - -af volume=0.1 %s", TEMP_AUDIO);
    pipe->audio = POPEN(command, "wb");
    pipe->recording_now = start_sample_count;
    
    return pipe;
}

void submit_to_recording(FFMPEGPipe *pipe, int *video_buffer, u8 *audio_buffer, int audio_length, u64 time_now) {
    
    fwrite(audio_buffer, AUDIO_CHANNELS * AUDIO_BITDEPTH, audio_length, pipe->audio);
    
    if(video_buffer) {
        while(time_now > pipe->recording_now) {
            fwrite(video_buffer, VIDEO_BPP, VIDEO_SIZE, pipe->video);
            // NOTE(stringflow): 2^21/60
            pipe->recording_now += 2097152.0 / 60.0;
        }
    }
}

void ffmpeg_close_pipe(FFMPEGPipe *pipe) {
    PCLOSE(pipe->video);
    PCLOSE(pipe->audio);
}

void ffmpeg_merge_video_audio(const char *video_title) {
    char command[256];
    sprintf(command, "ffmpeg -y -i %s -i %s -c:v copy -c:a copy -shortest %s", 
            TEMP_VIDEO, TEMP_AUDIO, video_title);
    system(command);
}

// Starts recording the emulator's video. Requires [ffmpeg](https://www.ffmpeg.org/) to be
// located in PATH to function properly. The recording dimensions must at least be 160x144,
// but may be larger causing upscaling of the video. Video and audio will be recorded
// seperately, calling gameboy_stoprecording will merge the two into a final file.
DLLEXPORT void gameboy_startrecording(GameBoy *gb, int width, int height) {
    if(gb->recording) return;
    gb->recording = ffmpeg_open_pipe(width, height, gb->samples_emulated);
}

// Stops the recording and merges the video and audio files into one final output saved
// to the specified file path. The video title must include the file extension.
DLLEXPORT void gameboy_stoprecording(GameBoy *gb, const char *video_title) {
    if(!gb->recording) return;
    ffmpeg_close_pipe(gb->recording);
    ffmpeg_merge_video_audio(video_title);
    delete gb->recording;
    gb->recording = nullptr;
}

// Saves a screenshot of the emulator's current video to the specified file. The only two
// codecs currently supported are `.bmp` and `.png`. Returns whether or not the file was
// saved correctly.
DLLEXPORT bool gameboy_screenshot(GameBoy *gb, const char *filename) {
    // TODO(stringflow): speedup flags check, maybe need to sneakily step so that the video
    // buffer isn't blank, only thing where this matters is running until
    // joypadoverworld + 1 instruction at the start of the movement code, I think.
    // Although the video buffer should only be filled on vblank, so a single step wouldn't
    // work, I believe. Need to test this.
    FILE *file = fopen(filename, "wb");
    if(!file) return false;
    
    bool format_supported = true;
    switch(hash(get_file_extension(filename))) {
        case hash("bmp"): {
            bmp_encode(file, gb->video_buffer, VIDEO_WIDTH, VIDEO_HEIGHT);
        } break;
        case hash("png"): {
            png_encode(file, gb->video_buffer, VIDEO_WIDTH, VIDEO_HEIGHT);
        } break;
        default: {
            format_supported = false;
        } break;
    }
    
    fclose(file);
    return format_supported;
}

// Sets a callback for whenever a new video frame is ready to be displayed.
// The callback will have access to the video buffer (R/W). This callback will be called
// **after** the optional grid overlay has already been drawn to the video buffer.
DLLEXPORT void gameboy_setvideocallback(GameBoy *gb, VideoCallback callback) {
    gb->video_callback = callback;
}

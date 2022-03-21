#ifndef VIDEO_H
#define VIDEO_H

struct FFMPEGPipe {
    FILE *video;
    FILE *audio;
    r64 recording_now;
};

#endif //VIDEO_H

#ifndef CODEC_BMP_H
#define CODEC_BMP_H

// NOTE(stringflow):
// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
#pragma pack(push, 1)
struct BMPHeader {
    u16 file_type;
    u32 file_size;
    u16 reserved1;
    u16 reserved2;
    u32 bitmap_offset;
    u32 size;
    s32 width;
    s32 height;
    u16 planes;
    u16 bits_per_pixel;
    u32 compression;
    u32 size_of_bitmap;
    s32 horizontal_resolution;
    s32 vertical_resolution;
    u32 colors_used;
    u32 colors_important;
};
#pragma pack(pop)

void bmp_encode(FILE *file, int *pixels, int width, int height) {
    u32 pixels_size = width * height * VIDEO_BPP;
    BMPHeader header = {
        .file_type = 0x4d42,
        .file_size = (u32) (sizeof(header) + pixels_size),
        .reserved1 = 0,
        .reserved2 = 0,
        .bitmap_offset = sizeof(header),
        .size = sizeof(header) - 14,
        .width = width,
        .height = -height, // NOTE(stringflow): same quirk as described in win32-window.
        .planes = 1,
        .bits_per_pixel = VIDEO_BPP * 8,
        .compression = 0,
        .size_of_bitmap = pixels_size,
        .horizontal_resolution = 0,
        .vertical_resolution = 0,
        .colors_used = 0,
        .colors_important = 0,
    };
    
    fwrite(&header, 1, sizeof(header), file);
    fwrite(pixels, VIDEO_BPP, width * height, file);
}

#endif //CODEC_BMP_H

#ifndef MAZE_BITMAP_H_
#define MAZE_BITMAP_H_

#include <string>
#include <vector>

// Pack these structs without any padding
#pragma pack(push, 1)

// Bitmap file header
struct FileHeader {
    uint16_t type = 0x4d42;
    uint32_t size;
    uint16_t reserved1 = 0;
    uint16_t reserved2 = 0;
    uint32_t imageDataOffset = 54;
};

// Bitmap DIB header (BITMAPINFOHEADER style)
struct DIBHeader {
    uint32_t thisHeaderSize = 40;
    uint32_t imageWidth;
    uint32_t imageHeight;
    uint16_t colorPlanes = 1;
    uint16_t bitsPerPixel;
    uint32_t compressionMethod = 0;
    uint32_t imageSize;
    uint32_t horResolution = 2835;
    uint32_t verResolution = 2835;
    uint32_t numColors = 0;
    uint32_t numImpColors = 0;
};

// Reset packing
#pragma pack(pop)

// 24 bit color
struct Color24 {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

// writeBitmap function
bool writeBitmap24(std::string, const std::vector<std::vector<Color24>>&);

#endif

#ifndef BITMAP_BITMAP_HPP_
#define BITMAP_BITMAP_HPP_

#include <array>
#include <string>
#include <vector>

#include "bitmap/image.hpp"

namespace bitmap {

// 24 bit color
struct Color24 {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

// Pack these structs without any padding
#pragma pack(push, 1)

// Bitmap file header
struct FileHeader {
    // "BM" as integer
    uint16_t type = 0x4d42; // NOLINT
    uint32_t size;
    uint16_t reserved1 = 0;
    uint16_t reserved2 = 0;
    uint32_t imageDataOffset;

    FileHeader(uint32_t imageSize, unsigned int colorTableEntries);
};

// Bitmap DIB header (BITMAPINFOHEADER style)
struct DIBHeader {
    uint32_t thisHeaderSize = 40; // NOLINT
    uint32_t imageWidth;
    uint32_t imageHeight;
    uint16_t colorPlanes = 1;
    uint16_t bitsPerPixel;
    uint32_t compressionMethod = 0;
    uint32_t imageSize;
    // 72 dpi printing
    uint32_t horResolution = 2835; // NOLINT
    uint32_t verResolution = 2835; // NOLINT
    uint32_t numColors = 0;
    uint32_t numImpColors = 0;

    DIBHeader(uint32_t imageWidth, uint32_t imageHeight, uint16_t bitsPerPixel);
};

// Offset for actual image data (size of both headers)
const unsigned int imageOffset = 54;

// Color table used for binary bitmap
const std::array<uint8_t, 8> colorTable = {
    255, 255, 255, 0,
    0,   0,   0,   0,
};

// Reset packing
#pragma pack(pop)

// writeBitmap function
void writeBitmap24(std::string, Image<Color24>&);

// write black and white bitmap function
void writeBitmapBW(std::string, Image<bool>&, bool);

} // namespace bitmap

#endif  // BITMAP_BITMAP_HPP_

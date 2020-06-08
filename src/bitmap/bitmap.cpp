#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <vector>

#include "bitmap/bitmap.hpp"
#include "bitmap/image.hpp"

namespace bitmap {

// Number of bits in a byte
const unsigned int numBits = 8;

// Number of bytes needed for row (without padding)
unsigned int getRowBytes(uint32_t imageWidth, uint16_t bitsPerPixel) {
    unsigned int rowBits = imageWidth * bitsPerPixel;
    unsigned int rowBytes = rowBits / numBits;
    // Leftover bits need a full byte
    if (rowBits % numBits != 0) {
        ++rowBytes;
    }
    return rowBytes;
}

// Internal function to get padding from row bytess
unsigned int getPaddingFromRow(unsigned int rowBytes) {
    if (rowBytes % 4 > 0) {
        return 4 - (rowBytes % 4);
    }
    return 0;
}

unsigned int getPaddingBytes(uint32_t imageWidth, uint16_t bitsPerPixel) {
    unsigned int rowBytes = getRowBytes(imageWidth, bitsPerPixel);
    return getPaddingFromRow(rowBytes);
}

DIBHeader::DIBHeader(uint32_t imageWidth, uint32_t imageHeight, uint16_t bitsPerPixel) :
    imageWidth(imageWidth),
    imageHeight(imageHeight),
    bitsPerPixel(bitsPerPixel) {
    unsigned int rowBytes = getRowBytes(imageWidth, bitsPerPixel);
    // Add padding to row bytes
    rowBytes += getPaddingFromRow(rowBytes);

    this->imageSize = rowBytes * imageHeight;
}

FileHeader::FileHeader(uint32_t imageSize, unsigned int colorTableEntries) :
    size(imageSize + imageOffset + (colorTableEntries * 4)),
    imageDataOffset(imageOffset + (colorTableEntries * 4)) {
}

unsigned int writeHeaders(std::ofstream& file, uint32_t imageWidth,
                          uint32_t imageHeight, uint16_t bitsPerPixel, int colorTableEntries) {
    // Setup headers
    DIBHeader dhead(imageWidth, imageHeight, bitsPerPixel);
    FileHeader fhead(dhead.imageSize, colorTableEntries);

    // Write headers
    file.write(reinterpret_cast<char*>(&fhead), sizeof(FileHeader)); // NOLINT: need to reinterpret
    file.write(reinterpret_cast<char*>(&dhead), sizeof(DIBHeader)); // NOLINT: need to reinterpret

    // Return bytes written
    return sizeof(FileHeader) + sizeof(DIBHeader);
}


// Write 24 bit color data to bitmap file (adding padding if neccesary)
void writeBitmap24(std::string fname, Image<Color24>& image) {
    std::ofstream file;
    // Open file for binary output
    file.open(fname, std::ios::out | std::ios::binary);

    int imageHeight = image.getNumRows();
    int imageWidth = image.getNumColumns();
    const unsigned int bitsPerPixel = 24;
    writeHeaders(file, imageWidth, imageHeight, bitsPerPixel, 0);
    // Write data
    unsigned int paddingBytes = getPaddingBytes(imageWidth, bitsPerPixel);
    for (int y = imageHeight - 1; y >= 0; --y) {
        for (int x = 0; x < imageWidth; ++x) {
            auto c = image.getValue(x, y);
            // Write colors
            file << c.blue << c.green << c.red;
        }
        // Write padding at end of a row
        for (unsigned int i = 0; i < paddingBytes; ++i) {
            file << static_cast<char>(0);
        }
    }
    file.close();
}

// Write 1 bit color data to bitmap file with padding
void writeBitmapBW(std::string fname, Image<bool>& image, bool verbose = false) {
    std::ofstream file;
    // Open file for binary output
    file.open(fname, std::ios::out | std::ios::binary);

    int imageHeight = image.getNumRows();
    int imageWidth = image.getNumColumns();
    if (verbose) {
        std::cout << "Width: " << imageWidth << "\n";
        std::cout << "Height: " << imageHeight << "\n";
    }
    unsigned int headerBytes = writeHeaders(file, imageWidth, imageHeight, 1, 2);
    if (verbose) {
        std::cout << "Wrote " << headerBytes << " header bytes\n";
    }

    // Write color table
    for (uint8_t tableEntry : colorTable) {
        file << static_cast<char>(tableEntry);
    }

    // Write data
    unsigned int paddingBytes = getPaddingBytes(imageWidth, 1);
    if (verbose) {
        std::cout << "Padding: " << paddingBytes << "\n";
    }
    int bytesWritten = 0;
    for (int y = imageHeight - 1; y >= 0; --y) {
        // Reset at start of row
        uint8_t toWrite = 0;
        int bitPos = 0;
        for (int x = 0; x < imageWidth; ++x) {
            bool on = image.getValue(x, y);
            // Write colors
            if (on) {
                // Set bit at bitPos in toWrite to 1
                toWrite |= (0x01 << (numBits - 1 - bitPos));
            }
            ++bitPos;
            // If toWrite is full, write to file
            if (bitPos == numBits) {
                file << toWrite;
                ++bytesWritten;
                toWrite = 0;
                bitPos = 0;
            }
        }
        // If bits left to be written
        if (bitPos != 0) {
            file << toWrite;
            ++bytesWritten;
        }
        // Write padding bytes to row
        for (unsigned int i = 0; i < paddingBytes; ++i) {
            file << static_cast<char>(0);
        }
    }
    if (verbose) {
        std::cout << "Wrote " << bytesWritten << " data bytes\n";
    }

    file.close();
    std::cout << "Saved to " << fname << "\n";
}

} // namespace bitmap

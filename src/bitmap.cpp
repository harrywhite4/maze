#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "bitmap.hpp"
#include "image.hpp"

int getPaddingBytes(uint32_t imageWidth, uint16_t bitsPerPixel) {
    int rowBits = imageWidth * bitsPerPixel;
    int rowBytes = rowBits / 8;
    if (rowBits % 8 != 0) {
        ++rowBytes;
    }
    return 4 - (rowBytes % 4);
}

DIBHeader::DIBHeader(uint32_t imageWidth, uint32_t imageHeight, uint16_t bitsPerPixel) :
    imageWidth(imageWidth),
    imageHeight(imageHeight),
    bitsPerPixel(bitsPerPixel) {
    int paddingBytes, rowBits, rowBytes;
    // Determine Bytes needed per row (with padding)
    rowBits = imageWidth * bitsPerPixel;
    rowBytes = rowBits / 8;
    // Leftover bits need a byte
    if (rowBits % 8 != 0) {
        ++rowBytes;
    }
    // Add padding
    paddingBytes = 4 - (rowBytes % 4);
    rowBytes += paddingBytes;

    // Set size
    this->imageSize = rowBytes * imageHeight;
}

FileHeader::FileHeader(uint32_t imageSize, unsigned int colorTableEntries) :
    size(imageSize + 54 + (colorTableEntries * 4)),
    imageDataOffset(54 + (colorTableEntries * 4)) {
}

void writeHeaders(std::ofstream& file, uint32_t imageWidth,
        uint32_t imageHeight, uint16_t bitsPerPixel, int colorTableEntries) {
    // Setup headers
    DIBHeader dhead(imageWidth, imageHeight, bitsPerPixel);
    FileHeader fhead(dhead.imageSize, colorTableEntries);
    std::cout << "Projected size: " << fhead.size << " bytes\n";

    // Write headers
    file.write((char*)&fhead, sizeof(FileHeader));
    file.write((char*)&dhead, sizeof(DIBHeader));
}


// Write 24 bit color data to bitmap file (adding padding if neccesary)
bool writeBitmap24(std::string fname, Image<Color24>& image) {
    // Get sizes
    unsigned int imageHeight = image.getNumRows();
    unsigned int imageWidth = image.getNumColumns();
    // Determine padding
    int paddingBytes = getPaddingBytes(imageWidth, 24);

    // Write to file
    std::ofstream file;
    // Open file for binary output
    file.open(fname, std::fstream::out | std::fstream::binary);
    // Write Headers
    writeHeaders(file, imageWidth, imageHeight, 24, 0);
    // Write data
    for (unsigned int y = 0; y < imageHeight; ++y) {
        for (unsigned int x = 0; x < imageWidth; ++x) {
            auto c = image.getValue(x, y);
            // Write colors
            file << c.blue << c.green << c.red;
        }
        // Write padding at end of a row
        for (int i = 0; i < paddingBytes; ++i) {
            file << 0;
        }
    }
    // Close file
    file.close();
    return true;
}

bool writeBitmapBW(std::string fname, Image<bool>& image) {
    // Get sizes
    unsigned int imageHeight = image.getNumRows();
    unsigned int imageWidth = image.getNumColumns();
    std::cout << "Width: " << imageWidth << "\n";
    std::cout << "Height: " << imageHeight << "\n";

    int paddingBytes = getPaddingBytes(imageWidth, 1);
    std::cout << "Padding: " << paddingBytes << "\n";

    // Write to file
    std::ofstream file;
    // Open file for binary output
    file.open(fname, std::fstream::out | std::fstream::binary);
    // Write Headers
    writeHeaders(file, imageWidth, imageHeight, 1, 2);
    // Write color table
    file << (char)0x00 << (char)0x00 << (char)0x00 << (char)0x00;
    file << (char)0xff << (char)0xff << (char)0xff << (char)0x00;
    // Write data
    uint8_t toWrite = 0;
    int bitPos = 0, bytesWritten = 0;
    for (unsigned int y = 0; y < imageHeight; ++y) {
        // Reset at start of row
        toWrite = 0;
        bitPos = 0;
        for (unsigned int x = 0; x < imageWidth; ++x) {
            bool on = image.getValue(x, y);
            // Write colors
            if (on) {
                // Set bit at bitPos in toWrite to 1
                toWrite |= (0x01 << (7 - bitPos));
            }
            ++bitPos;
            if (bitPos == 8) {
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
        // Write padding
        for (int i = 0; i < paddingBytes; ++i) {
            file << 0;
        }
    }
    std::cout << "Wrote " << bytesWritten << " data bytes\n";
    // Close file
    file.close();
    return true;
}

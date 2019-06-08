#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "bitmap.hpp"


template <typename T>
bool validateSize(unsigned int size, const std::vector<std::vector<T>>& vec) {
    for (auto row : vec) {
        if (row.size() != size) {
            return false;
        }
    }
    return true;
}

int getPadding(uint32_t imageWidth, uint16_t bitsPerPixel) {
    int rowBits = imageWidth * bitsPerPixel;
    int rowBytes = rowBits / 8;
    if (rowBits % 8 != 0) {
        ++rowBytes;
    }
    return 4 - (rowBytes % 4);
}

void writeHeaders(std::ofstream& file, uint32_t imageWidth,
        uint32_t imageHeight, uint16_t bitsPerPixel, int colorTableEntries) {
    int paddingBytes, rowBits, rowBytes;
    // Determine padding
    paddingBytes = getPadding(imageWidth, bitsPerPixel);
    // Determine Bytes needed per row (with padding)
    rowBits = imageWidth * bitsPerPixel;
    rowBytes = rowBits / 8;
    // Leftover bits need a byte
    if (rowBits % 8 != 0) {
        ++rowBytes;
    }
    // Add padding
    rowBytes += paddingBytes;

    // Setup headers
    DIBHeader dhead;

    dhead.imageWidth = imageWidth;
    dhead.imageHeight = imageHeight;
    dhead.bitsPerPixel = bitsPerPixel;
    dhead.imageSize = rowBytes * imageHeight;

    FileHeader fhead;
    fhead.imageDataOffset = 54 + (colorTableEntries * 4);
    fhead.size = dhead.imageSize + 54 + (colorTableEntries * 4);
    std::cout << "Projected size: " << fhead.size << " bytes\n";

    // Write headers
    file.write((char*)&fhead, sizeof(FileHeader));
    file.write((char*)&dhead, sizeof(DIBHeader));
}


// Write 24 bit color data to bitmap file (adding padding if neccesary)
bool writeBitmap24(std::string fname, const std::vector<std::vector<Color24>>& data) {
    // Get sizes
    unsigned int imageHeight = data.size();
    unsigned int imageWidth = data[0].size();
    // make sure input contains vectors of same length
    if (!validateSize(imageWidth, data)) {
        return false;
    }
    // Determine padding
    int paddingBytes = getPadding(imageWidth, 24);

    // Write to file
    std::ofstream file;
    // Open file for binary output
    file.open(fname, std::fstream::out | std::fstream::binary);
    // Write Headers
    writeHeaders(file, imageWidth, imageHeight, 24, 0);
    // Write data
    for (auto row : data) {
        // Write colors
        for (auto c : row) {
            file << c.blue << c.green << c.red;
        }
        // Write padding
        for (int i = 0; i < paddingBytes; ++i) {
            file << 0;
        }
    }
    // Close file
    file.close();
    return true;
}

bool writeBitmapBW(std::string fname, const std::vector<std::vector<bool>>& data) {
    // Get sizes
    unsigned int imageHeight = data.size();
    unsigned int imageWidth = data[0].size();
    std::cout << "Width: " << imageWidth << "\n";
    std::cout << "Height: " << imageHeight << "\n";
    // make sure input contains vectors of same length
    if (!validateSize(imageWidth, data)) {
        return false;
    }

    int paddingBytes = getPadding(imageWidth, 1);
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
    for (auto row : data) {
        // Write colors
        toWrite = 0;
        bitPos = 0;
        for (bool on : row) {
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

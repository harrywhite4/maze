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
    int paddingBytes = imageWidth % 4;

    // Setup headers
    DIBHeader dhead;
    // TODO include padding in this
    dhead.imageSize = imageWidth * imageHeight * 3;
    dhead.imageWidth = imageWidth;
    dhead.imageHeight = imageHeight;
    dhead.bitsPerPixel = 24;

    FileHeader fhead;
    fhead.size = dhead.imageSize + 54;

    // Write to file
    std::ofstream file;
    // Open file for binary output
    file.open(fname, std::fstream::out | std::fstream::binary);
    // Write Headers
    file.write((char*)&fhead, sizeof(FileHeader));
    file.write((char*)&dhead, sizeof(DIBHeader));
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
    // make sure input contains vectors of same length
    if (!validateSize(imageWidth, data)) {
        return false;
    }

    // Setup headers
    DIBHeader dhead;
    dhead.imageSize = (imageWidth * imageHeight) / 8;
    dhead.imageWidth = imageWidth;
    dhead.imageHeight = imageHeight;
    dhead.bitsPerPixel = 1;

    FileHeader fhead;
    fhead.imageDataOffset = 54 + 8;
    fhead.size = dhead.imageSize + fhead.imageDataOffset;
    std::cout << "Projected size: " << fhead.size << " bytes\n";
    
    // Write to file
    std::ofstream file;
    // Open file for binary output
    file.open(fname, std::fstream::out | std::fstream::binary);
    // Write Headers
    file.write((char*)&fhead, sizeof(FileHeader));
    file.write((char*)&dhead, sizeof(DIBHeader));
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
                toWrite |= (0x01 << bitPos);
            }
            ++bitPos;
            if (bitPos == 8) {
                file << toWrite;
                ++bytesWritten;
                toWrite = 0;
                bitPos = 0;
            }
        }
    }
    std::cout << "Wrote " << bytesWritten << " data bytes\n";
    // Close file
    file.close();
    return true;
}

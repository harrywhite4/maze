#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "bitmap.hpp"
#include "image.hpp"

// Number of bytes needed for row (without padding)
int getRowBytes(uint32_t imageWidth, uint16_t bitsPerPixel) {
    int rowBits = imageWidth * bitsPerPixel;
    int rowBytes = rowBits / 8;
    // Leftover bits need a full byte
    if (rowBits % 8 != 0) {
        ++rowBytes;
    }
    return rowBytes;
}

// Internal function to get padding from row bytes
int getPaddingFromRow(int rowBytes) {
    if (rowBytes % 4 > 0) {
        return 4 - (rowBytes % 4);
    }
    return 0;
}

int getPaddingBytes(uint32_t imageWidth, uint16_t bitsPerPixel) {
    int rowBytes = getRowBytes(imageWidth, bitsPerPixel);
    return getPaddingFromRow(rowBytes);
}

DIBHeader::DIBHeader(uint32_t imageWidth, uint32_t imageHeight, uint16_t bitsPerPixel) :
    imageWidth(imageWidth),
    imageHeight(imageHeight),
    bitsPerPixel(bitsPerPixel) {
    int rowBytes = getRowBytes(imageWidth, bitsPerPixel);
    // Add padding to row bytes
    rowBytes += getPaddingFromRow(rowBytes);

    this->imageSize = rowBytes * imageHeight;
}

FileHeader::FileHeader(uint32_t imageSize, unsigned int colorTableEntries) :
    size(imageSize + 54 + (colorTableEntries * 4)),
    imageDataOffset(54 + (colorTableEntries * 4)) {
}

unsigned int writeHeaders(std::ofstream& file, uint32_t imageWidth,
        uint32_t imageHeight, uint16_t bitsPerPixel, int colorTableEntries) {
    // Setup headers
    DIBHeader dhead(imageWidth, imageHeight, bitsPerPixel);
    FileHeader fhead(dhead.imageSize, colorTableEntries);

    // Write headers
    file.write(reinterpret_cast<char*>(&fhead), sizeof(FileHeader));
    file.write(reinterpret_cast<char*>(&dhead), sizeof(DIBHeader));

    // Return bytes written
    return sizeof(FileHeader) + sizeof(DIBHeader);
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

bool writeBitmapBW(std::string fname, Image<bool>& image, bool verbose = false) {
    // Get sizes
    unsigned int imageHeight = image.getNumRows();
    unsigned int imageWidth = image.getNumColumns();
    if (verbose) {
        std::cout << "Width: " << imageWidth << "\n";
        std::cout << "Height: " << imageHeight << "\n";
    }

    int paddingBytes = getPaddingBytes(imageWidth, 1);
    if (verbose) {
        std::cout << "Padding: " << paddingBytes << "\n";
    }

    // Write to file
    std::ofstream file;
    // Open file for binary output
    file.open(fname, std::fstream::out | std::fstream::binary);
    // Write Headers
    auto headerBytes = writeHeaders(file, imageWidth, imageHeight, 1, 2);
    if (verbose) {
        std::cout << "Wrote " << headerBytes << " header bytes\n";
    }
    // Write color table
    file << static_cast<char>(0x00) << static_cast<char>(0x00)
         << static_cast<char>(0x00) << static_cast<char>(0x00);
    file << static_cast<char>(0xff) << static_cast<char>(0xff)
         << static_cast<char>(0xff) << static_cast<char>(0x00);
    // Write data
    uint8_t toWrite = 0;
    int bitPos = 0, bytesWritten = 0;
    // TODO Write in correct vertical order
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
    if (verbose) {
        std::cout << "Wrote " << bytesWritten << " data bytes\n";
    }
    // Close file
    file.close();
    std::cout << "Saved to " << fname << "\n";
    return true;
}

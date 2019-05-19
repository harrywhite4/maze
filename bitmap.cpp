#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "bitmap.hpp"


// Write 24 bit color data to bitmap file (adding padding if neccesary)
bool writeBitmap24(std::string fname, const std::vector<std::vector<Color24>>& data) {
    // Get sizes
    unsigned int imageHeight = data.size();
    unsigned int imageWidth = data[0].size();
    // make sure input contains vectors of same length
    for (auto row : data) {
        if (row.size() != imageWidth) {
            return false;
        }
    }
    // Determine padding
    int paddingBytes = imageWidth % 4;

    // Setup headers
    DIBHeader dhead;
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

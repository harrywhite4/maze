#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "bitmap.hpp"


// Write image data to bitmap file (adding padding if neccesary
bool writeBitmap(std::string fname, const FileHeader& fhead, const DIBHeader& dhead, const std::vector<std::vector<uint8_t>>& data) {
    // TODO make sure input contains vectors of same length
    // Write to file
    std::ofstream file;
    // Open file for binary output
    file.open(fname, std::fstream::out | std::fstream::binary);
    // Write Headers
    file.write((char*)&fhead, sizeof(FileHeader));
    file.write((char*)&dhead, sizeof(DIBHeader));
    // Write data
    int paddingBytes = data[0].size() % 4;
    for (std::vector<uint8_t> row : data) {
        // Write data
        for (uint8_t el : row) {
            file << el;
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

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const std::string usage = "Usage: bitmap fname\n";

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

// Write image data to bitmap file (adding padding if neccesary
bool writeBitmap(std::string fname, const FileHeader &fhead, const DIBHeader &dhead, const std::vector<std::vector<uint8_t>> &data) {
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

int main(int argc, char *argv[]) {
    // Argument parsing
    if (argc < 2) {
        std::cerr << usage;
        exit(EXIT_FAILURE);
    }
    int imgWidth = 1000, imgHeight = 1000;
    std::string fname = argv[1];

    // Setup headers
    FileHeader fhead;

    DIBHeader dhead;
    dhead.imageWidth = imgWidth;
    dhead.imageHeight = imgHeight;
    dhead.bitsPerPixel = 24;

    // Build data
    int rowWidth = imgWidth * 3;
    std::vector<std::vector<uint8_t>> data(imgHeight, std::vector<uint8_t>(rowWidth, 0));

    for (int r = 0; r < imgHeight; ++r) {
        for (int i = 0; i < rowWidth; i += 3) {
            // TODO use color struct
            // Blue
            data[r][i] = r % 256;
            // Green
            data[r][i + 1] = 255 - (r % 256);
            // Red
            data[r][i + 2] = r % 100;
        }
    }

    // Set header sizes
    dhead.imageSize = imgHeight * rowWidth;
    fhead.size = dhead.imageSize + 54;

    // Write bmp
    std::cout << "Writing to " << fname << "\n";
    bool success = writeBitmap(fname, fhead, dhead, data);

    if (!success) {
        std::cout << "Could not write to file\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Done!\n";
    exit(EXIT_SUCCESS);

}

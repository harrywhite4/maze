#include <string>
#include <vector>
#include <iostream>

#include "bitmap.hpp"

const std::string usage = "Usage: bitmap fname\n";

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

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

    // Build data
    std::vector<std::vector<Color24>> data(imgHeight, std::vector<Color24>(imgWidth));

    for (int r = 0; r < imgHeight; ++r) {
        for (int i = 0; i < imgWidth; ++i) {
            data[r][i].red = r % 100;
            data[r][i].green = 255 - (r % 256);
            data[r][i].blue = r % 256;
        }
    }

    // Write bmp
    std::cout << "Writing to " << fname << "\n";
    bool success = writeBitmap24(fname, data);

    if (!success) {
        std::cout << "Could not write to file\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Done!\n";
    exit(EXIT_SUCCESS);
}

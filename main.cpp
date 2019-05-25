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
    int imgWidth = 32, imgHeight = 32;
    std::string fname = argv[1];

    // Build data
    // std::vector<std::vector<Color24>> data(imgHeight, std::vector<Color24>(imgWidth));
    // for (int r = 0; r < imgHeight; ++r) {
    //     for (int i = 0; i < imgWidth; ++i) {
    //         if ((i % 20 > 9) || (r % 20 > 9)) {
    //             data[r][i].blue = 0;
    //         } else {
    //             data[r][i].blue = 255;
    //         }
    //         data[r][i].red = i*r % 256;
    //         data[r][i].green = 256 - (i*r % 256);
    //     }
    // }
    std::vector<std::vector<bool>> data(imgHeight, std::vector<bool>(imgWidth, false));
    for (int r = 0; r < imgHeight; ++r) {
        for (int i = 0; i < imgWidth; ++i) {
            if (i % 2 == 0) {
                data[r][i] = true;
            }
        }
    }

    // Write bmp
    std::cout << "Writing to " << fname << "\n";
    // bool success = writeBitmap24(fname, data);
    bool success = writeBitmapBW(fname, data);

    if (!success) {
        std::cout << "Could not write to file\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Done!\n";
    exit(EXIT_SUCCESS);
}

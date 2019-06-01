#include <string>
#include <vector>
#include <iostream>

#include "bitmap.h"

const std::string usage = "Usage: bitmap [options] fname\nOptions:"
                          "\n--color Use color image\n"
                          "--bw Use black and white image\n"
                          "--help Print help\n";

int main(int argc, char *argv[]) {
    // Variables
    bool BlackWhite = true, success;
    int imgWidth, imgHeight;
    // Argument parsing
    std::vector<std::string> posArgs, flagArgs;
    std::string current;
    for (int i = 1; i < argc; ++i) {
        current = argv[i];
        if (current[0] == '-') {
            flagArgs.push_back(current);
        } else {
            posArgs.push_back(current);
        }
    }

    // Set based on flagArgs
    for (auto flag : flagArgs) {
        if (flag == "--bw") {
            BlackWhite = true;
        } else if (flag == "--color") {
            BlackWhite = false;
        } else if (flag == "--help") {
            std::cout << usage;
            exit(EXIT_SUCCESS);
        } else {
            std::cerr << "Invalid argument " << flag << "\n" << usage;
            exit(EXIT_FAILURE);
        }
    }

    // Set based on posArgs
    if (posArgs.size() != 1) {
        std::cerr << "Filename positional argument required\n" << usage;
        exit(EXIT_FAILURE);
    }
    std::string fname = posArgs[0];

    // Build data
    if (!BlackWhite) {
        imgWidth = 1000;
        imgHeight = 1000;
        std::vector<std::vector<Color24>> data(imgHeight, std::vector<Color24>(imgWidth));
        for (int r = 0; r < imgHeight; ++r) {
            for (int i = 0; i < imgWidth; ++i) {
                if ((i % 20 > 9) || (r % 20 > 9)) {
                    data[r][i].blue = 0;
                } else {
                    data[r][i].blue = 255;
                }
                data[r][i].red = i*r % 256;
                data[r][i].green = 256 - (i*r % 256);
            }
        }
        // Write bmp
        std::cout << "Writing to " << fname << "\n";
        success = writeBitmap24(fname, data);
    } else {
        imgWidth = 32;
        imgHeight = 32;
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
        success = writeBitmapBW(fname, data);
    }

    if (!success) {
        std::cout << "Could not write to file\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Done!\n";
    exit(EXIT_SUCCESS);
}

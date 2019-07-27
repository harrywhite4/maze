#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <exception>

#include "bitmap.hpp"
#include "grid.hpp"
#include "maze.hpp"
#include "argparse.hpp"
#include "image.hpp"

const char usage[] = "Usage: maze [options]\nOptions:\n"
                     "-o Output filename (default: \"maze.bmp\")\n"
                     "-w Maze width (default: 50)\n"
                     "-h Maze height (default: 50)\n"
                     "--text Output maze as text to stdout\n"
                     "--verbose Print detailed output\n"
                     "--help Print help\n";

// validate dimension, exiting if not valid
void validateDimension(int dimension, std::string name) {
    if (dimension <= 0) {
        std::cerr << name << " must be a positive number\n";
        exit(EXIT_FAILURE);
    }
}

// string to int, 0 if not able to convert
int stringToInt(std::string text) {
    try {
        return std::stoi(text);
    } catch(std::exception&) {
        return 0;
    }
}

int main(int argc, char *argv[]) {
    // Variables
    bool parseSuccess;
    std::string fname;
    unsigned int width, height;
    // Argument parsing
    auto parser = ArgumentParser();
    parser.addFlagArg("--help", false);
    parser.addFlagArg("--text", false);
    parser.addFlagArg("--verbose", false);
    parser.addParamArg("-o", "maze.bmp");
    parser.addParamArg("-w", "50");
    parser.addParamArg("-h", "50");
    parseSuccess = parser.parse(argc, argv);

    if (!parseSuccess) {
        std::cout << usage;
        exit(EXIT_FAILURE);
    }
    if (parser.getFlag("--help")) {
        std::cout << usage;
        exit(EXIT_SUCCESS);
    }
    fname = parser.getParameter("-o");
    // Convert width / height & validate
    width = stringToInt(parser.getParameter("-w"));
    height = stringToInt(parser.getParameter("-h"));
    validateDimension(width, "width");
    validateDimension(height, "height");

    // Build data
    bool success;
    auto graph = GridGraph(height, width);
    lerwGraph(graph);
    if (parser.getFlag("--text")) {
        std::cout << graphToText(graph);
        success = true;
    } else {
        Image<bool> image = graphToImage(graph);
        success = writeBitmapBW(fname, image, parser.getFlag("--verbose"));
    }

    if (!success) {
        std::cout << "Could not write to file\n";
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

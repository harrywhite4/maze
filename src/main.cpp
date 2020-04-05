#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <exception>

#include "MazeConfig.h"
#include "bitmap/bitmap.hpp"
#include "bitmap/image.hpp"
#include "mazelib/grid.hpp"
#include "mazelib/maze.hpp"
#include "cxxopts.hpp"

// validate dimension, exiting if not valid
void validateDimension(int dimension, std::string name) {
    if (dimension <= 0) {
        std::cerr << name << " must be a positive number\n";
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    // Argument definition
    auto options = cxxopts::Options("maze", MAZE_DESCRIPTION);
    options.add_options()
        ("o,output", "Output filename", cxxopts::value<std::string>()->default_value("maze.bmp"))
        ("w,width", "Maze width", cxxopts::value<int>()->default_value("50"))
        ("h,height", "Maze height", cxxopts::value<int>()->default_value("50"))
        ("text", "Output maze as text to stdout", cxxopts::value<bool>()->default_value("false"))
        ("verbose", "Print detailed output", cxxopts::value<bool>()->default_value("false"))
        ("version", "Print version information", cxxopts::value<bool>()->default_value("false"))
        ("help", "Print help", cxxopts::value<bool>()->default_value("false"))
        ;

    // Variables
    std::string fname;
    int width, height;
    bool help, text, verbose, version;
    // Parse arguments
    try {
        cxxopts::ParseResult result = options.parse(argc, argv);

        fname = result["output"].as<std::string>();
        width = result["width"].as<int>();
        height = result["width"].as<int>();
        help = result["help"].as<bool>();
        text = result["text"].as<bool>();
        verbose = result["verbose"].as<bool>();
        version = result["version"].as<bool>();
    } catch (cxxopts::OptionParseException& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Validation
    validateDimension(width, "width");
    validateDimension(height, "height");

    // Show help
    if (help) {
        std::cout << options.help() << std::endl;
        exit(EXIT_SUCCESS);
    }
    // Show version info
    if (version) {
        std::cout << "maze " << MAZE_VERSION << std::endl;
        exit(EXIT_SUCCESS);
    }

    // Build data
    bool success;
    auto graph = GridGraph(height, width);
    lerwGraph(graph);
    if (text) {
        std::cout << graphToText(graph);
        success = true;
    } else {
        Image<bool> image = graphToImage(graph);
        success = writeBitmapBW(fname, image, verbose);
    }

    if (!success) {
        std::cout << "Could not write to file\n";
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

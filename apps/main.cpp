#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <exception>

#include "bitmap.hpp"
#include "grid.hpp"
#include "maze.hpp"
#include "image.hpp"
#include "cxxopts.hpp"

// validate dimension, exiting if not valid
void validateDimension(int dimension, std::string name) {
    if (dimension <= 0) {
        std::cerr << name << " must be a positive number\n";
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    // Argument parsing
    auto options = cxxopts::Options("maze", "Generate mazes");
    options.add_options()
        ("o,output", "Output filename", cxxopts::value<std::string>()->default_value("maze.bmp"))
        ("w,width", "Maze width", cxxopts::value<int>()->default_value("50"))
        ("h,height", "Maze height", cxxopts::value<int>()->default_value("50"))
        ("text", "Output maze as text to stdout", cxxopts::value<bool>()->default_value("false"))
        ("verbose", "Print detailed output", cxxopts::value<bool>()->default_value("false"))
        ("help", "Print help")
    ;
    
    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(EXIT_SUCCESS);
    }
    std::string fname = result["output"].as<std::string>();
    // Convert width / height & validate
    int width = result["width"].as<int>();
    int height = result["width"].as<int>();
    validateDimension(width, "width");
    validateDimension(height, "height");

    // Build data
    bool success;
    auto graph = GridGraph(height, width);
    lerwGraph(graph);
    if (result.count("text")) {
        std::cout << graphToText(graph);
        success = true;
    } else {
        Image<bool> image = graphToImage(graph);
        success = writeBitmapBW(fname, image, result["verbose"].as<bool>());
    }

    if (!success) {
        std::cout << "Could not write to file\n";
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

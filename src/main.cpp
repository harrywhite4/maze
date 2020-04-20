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

void createMaze(mazelib::GridGraph& graph, bool dfs) {
    if (dfs) {
        mazelib::dfsGraph(graph);
    } else {
        mazelib::lerwGraph(graph);
    }
}

void outputMaze(const mazelib::GridGraph& maze, std::string fname, bool text, bool verbose) {
    if (text) {
        std::cout << graphToText(maze);
    } else {
        bitmap::Image<bool> image = graphToImage(maze);
        bitmap::writeBitmapBW(fname, image, verbose);
    }
}

int main(int argc, char *argv[]) {
    // Argument definition
    cxxopts::Options options("maze", MAZE_DESCRIPTION);
    options.add_options()
        ("o,output", "Output filename", cxxopts::value<std::string>()->default_value("maze.bmp"))
        ("w,width", "Maze width", cxxopts::value<int>()->default_value("50"))
        ("h,height", "Maze height", cxxopts::value<int>()->default_value("50"))
        ("text", "Output maze as text to stdout", cxxopts::value<bool>())
        ("dfs", "Use dfs to create maze", cxxopts::value<bool>())
        ("verbose", "Print detailed output", cxxopts::value<bool>())
        ("version", "Print version information", cxxopts::value<bool>())
        ("help", "Print help", cxxopts::value<bool>());

    // Variables
    std::string fname;
    int width, height;
    bool help, text, dfs, verbose, version;
    // Parse arguments
    try {
        cxxopts::ParseResult result = options.parse(argc, argv);

        fname = result["output"].as<std::string>();
        width = result["width"].as<int>();
        height = result["height"].as<int>();
        help = result["help"].as<bool>();
        text = result["text"].as<bool>();
        dfs = result["dfs"].as<bool>();
        verbose = result["verbose"].as<bool>();
        version = result["version"].as<bool>();
    } catch (cxxopts::OptionParseException& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Show help
    if (help) {
        std::cout << options.help() << "\n";
        exit(EXIT_SUCCESS);
    }
    // Show version info
    if (version) {
        std::cout << "maze " << MAZE_VERSION << "\n";
        exit(EXIT_SUCCESS);
    }

    // Validation
    validateDimension(width, "width");
    validateDimension(height, "height");

    // Build data
    mazelib::GridGraph graph(height, width);
    try {
        createMaze(graph, dfs);
    } catch (std::exception& e) {
        std::cerr << "An error occurred while creating maze :(\n";
        std::cerr << e.what() << "\n";
        exit(EXIT_FAILURE);
    }

    try {
        outputMaze(graph, fname, text, verbose);
    } catch (std::exception& e) {
        std::cerr << "An error occurred while outputting maze :(\n";
        std::cerr << e.what() << "\n";
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

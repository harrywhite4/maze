#include <exception>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "MazeConfig.h"
#include "mazelib/grid.hpp"
#include "mazelib/maze.hpp"
#include "cxxopts.hpp"

// Get keys of map as a string of options in brackets
template <typename T>
std::string getOptionsList(std::map<std::string, T> mapping) {
    std::string opts = "[";
    bool firstOpt = true;
    for (auto item : mapping) {
        if (!firstOpt) {
            opts += ", ";
        } else {
            firstOpt = false;
        }
        opts += item.first;
    }
    opts += "]";
    return opts;
}

// validate dimension, exiting if not valid
void validateDimension(int dimension, std::string name) {
    if (dimension <= 0) {
        std::cerr << name << " must be a positive number\n";
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    // Argument definition
    cxxopts::Options options("maze", MAZE_DESCRIPTION);
    options.add_options()
        ("o,output", "Output filename", cxxopts::value<std::string>()->default_value("maze.bmp"))
        ("w,width", "Maze width", cxxopts::value<int>()->default_value("50"))
        ("h,height", "Maze height", cxxopts::value<int>()->default_value("50"))
        ("f,format", "Output format " + getOptionsList(mazelib::outputFormatMap),
         cxxopts::value<std::string>()->default_value("text"))
        ("t,type", "Maze type " + getOptionsList(mazelib::mazeTypeMap),
         cxxopts::value<std::string>()->default_value("wilsons"))
        ("verbose", "Print detailed output", cxxopts::value<bool>())
        ("version", "Print version information", cxxopts::value<bool>())
        ("help", "Print help", cxxopts::value<bool>());

    // Variables
    std::string fname, formatText, typeText;
    int width, height;
    bool help, verbose, version;
    // Parse arguments
    try {
        cxxopts::ParseResult result = options.parse(argc, argv);

        fname = result["output"].as<std::string>();
        width = result["width"].as<int>();
        height = result["height"].as<int>();
        formatText = result["format"].as<std::string>();
        typeText = result["type"].as<std::string>();
        verbose = result["verbose"].as<bool>();
        version = result["version"].as<bool>();
        help = result["help"].as<bool>();
    } catch (cxxopts::OptionParseException& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Get enums from mappings
    mazelib::MazeType type = mazelib::mazeTypeMap.at(typeText);
    mazelib::OutputFormat format = mazelib::outputFormatMap.at(formatText);

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
        mazelib::createMaze(graph, type);
    } catch (std::exception& e) {
        std::cerr << "An error occurred while creating maze :(\n";
        std::cerr << e.what() << "\n";
        exit(EXIT_FAILURE);
    }

    try {
        outputMaze(graph, format, fname, verbose);
    } catch (std::exception& e) {
        std::cerr << "An error occurred while outputting maze :(\n";
        std::cerr << e.what() << "\n";
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

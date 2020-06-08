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

// Parse command line arguments
// NOLINTNEXTLINE: Keeping argv as C array
cxxopts::ParseResult parseArgs(int argc, char *argv[]) {
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

    auto result = options.parse(argc, argv);

    bool help = result["help"].as<bool>();
    if (help) {
        std::cout << options.help() << "\n";
        exit(EXIT_SUCCESS);
    }

    bool version = result["version"].as<bool>();
    if (version) {
        std::cout << "maze " << MAZE_VERSION << "\n";
        exit(EXIT_SUCCESS);
    }

    return result;
}

// Parse arguments and output maze
void maze(cxxopts::ParseResult args) {
    int width = args["width"].as<int>();
    validateDimension(width, "width");
    int height = args["height"].as<int>();
    validateDimension(height, "height");

    // Get enums from mappings
    auto formatText = args["format"].as<std::string>();
    auto typeText = args["type"].as<std::string>();
    mazelib::MazeType type = mazelib::mazeTypeMap.at(typeText);
    mazelib::OutputFormat format = mazelib::outputFormatMap.at(formatText);

    mazelib::GridGraph graph(height, width);
    mazelib::createMaze(graph, type);

    auto fname = args["output"].as<std::string>();
    auto verbose = args["verbose"].as<bool>();
    outputMaze(graph, format, fname, verbose);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    try {
        auto args = parseArgs(argc, argv);
        maze(args);
    } catch (cxxopts::OptionParseException& e) {
        std::cerr << e.what() << "\n";
        exit(EXIT_FAILURE);
    } catch (std::exception& e) {
        std::cerr << "An error has occurred\n";
        exit(EXIT_FAILURE);
    }
}

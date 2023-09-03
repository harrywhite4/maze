#include <exception>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "MazeConfig.h"
#include "mazelib/error.hpp"
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

// Build options object
cxxopts::Options getOptions() {
    // Argument definition
    cxxopts::Options options("maze", MAZE_DESCRIPTION);
    options.add_options()
        ("o,output", "Output filename", cxxopts::value<std::string>()->default_value("maze.bmp"))
        ("w,width", "Maze width", cxxopts::value<int>()->default_value("50"))
        ("h,height", "Maze height", cxxopts::value<int>()->default_value("50"))
        ("f,format", "Output format [bitmap, text]",
         cxxopts::value<mazelib::OutputFormat>()->default_value("text"))
        ("t,type", "Maze type [dfs, wilsons]",
         cxxopts::value<mazelib::MazeType>()->default_value("wilsons"))
        ("verbose", "Print detailed output", cxxopts::value<bool>())
        ("version", "Print version information", cxxopts::value<bool>())
        ("help", "Print help", cxxopts::value<bool>());
    return options;
}

cxxopts::ParseResult parseArgs(cxxopts::Options& options, int argc, char **argv) {
    auto args = options.parse(argc, argv);

    // Print help and exit on --help
    bool help = args["help"].as<bool>();
    if (help) {
        std::cout << options.help() << "\n";
        exit(EXIT_SUCCESS);
    }

    // Print version and exit on --version
    bool version = args["version"].as<bool>();
    if (version) {
        std::cout << "maze " << MAZE_VERSION << "\n";
        exit(EXIT_SUCCESS);
    }

    // Validate width and height
    validateDimension(args["width"].as<int>(), "width");
    validateDimension(args["height"].as<int>(), "height");
    return args;
}

// Build maze based on command line args
mazelib::GridGraph buildMaze(const cxxopts::ParseResult& args) {
    auto type = args["type"].as<mazelib::MazeType>();
    int width = args["width"].as<int>();
    int height = args["height"].as<int>();

    mazelib::GridGraph graph(height, width);
    mazelib::createMaze(graph, type);
    return graph;
}

// Show maze in some format based on args
void showMaze(const mazelib::GridGraph& graph, const cxxopts::ParseResult& args) {
    // Print maze
    auto format = args["format"].as<mazelib::OutputFormat>();
    auto fname = args["output"].as<std::string>();
    auto verbose = args["verbose"].as<bool>();
    outputMaze(graph, format, fname, verbose);
}

// Create and output maze based on args
void maze(cxxopts::Options& options, int argc, char** argv) {
    try {
        auto args = parseArgs(options, argc, argv);
        auto graph = buildMaze(args);
        showMaze(graph, args);
    } catch (cxxopts::exceptions::parsing& e) {
        // Handle option parsing error, printing help
        std::cerr << e.what() << "\n\n";
        std::cout << options.help() << "\n";
        exit(EXIT_FAILURE);
    } catch (mazelib::MazeError& e) {
        std::cerr << "An error has occurred while constructing maze\n";
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv) {
    try {
        // Build options here since we need it to output help on parse errors
        cxxopts::Options options = getOptions();
        maze(options, argc, argv);
    } catch (std::exception& e) {
        // Catch any exception at top level
        std::cerr << "An error has occurred\n";
        exit(EXIT_FAILURE);
    }
}

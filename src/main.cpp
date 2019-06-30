#include <string>
#include <vector>
#include <iostream>
#include <random>

#include "bitmap.hpp"
#include "grid.hpp"
#include "maze.hpp"
#include "argparse.hpp"
#include "image.hpp"

const char usage[] = "Usage: maze [options]\nOptions:\n"
                      "-o Output filename (default: \"maze.bmp\")\n"
                      "-w Maze width (default: 50)\n"
                      "-h Maze height (default: 50)\n"
                      "--help Print help\n";

int main(int argc, char *argv[]) {
    // Variables
    bool parseSuccess;
    std::string fname;
    unsigned int width, height;
    // Argument parsing
    auto parser = ArgumentParser();
    parser.addFlagArg("--help", false);
    parser.addFlagArg("--text", false);
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
    // TODO Handle conversion errors
    width = std::stoul(parser.getParameter("-w"));
    height = std::stoul(parser.getParameter("-h"));

    // Build data
    bool success;
    auto graph = GridGraph(height, width);
    lerwGraph(graph);
    if (parser.getFlag("--text")) {
        std::cout << graphToText(graph);
        success = true;
    } else {
        Image<bool> image = graphToImage(graph);
        success = writeBitmapBW(fname, image);
    }

    if (!success) {
        std::cout << "Could not write to file\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Done!\n";
    exit(EXIT_SUCCESS);
}

#include <map>
#include <string>
#include <iostream>

#include "argparse.hpp"

// TODO replace with cxxoptshttps://github.com/jarro2783/cxxopts

void ArgumentParser::addParamArg(std::string option, std::string def) {
    paramArgs[option] = def;
}

void ArgumentParser::addFlagArg(std::string option, bool def) {
    flagArgs[option] = def;
}

bool ArgumentParser::parse(int argc, char *argv[]) {
    int argNum = 1;
    std::string currentArg;
    while (argNum < argc) {
        currentArg = argv[argNum];
        if (paramArgs.count(currentArg) > 0) {
            // If it's a param arg
            if (argNum + 1 < argc) {
                paramArgs[currentArg] = argv[argNum+1];
                ++argNum;
            }
        } else if (flagArgs.count(currentArg) > 0) {
            // If it's a flag arg
            flagArgs[currentArg] = !flagArgs[currentArg];
        } else {
            std::cerr << "Unknown arg " << currentArg << "\n";
            return false;
        }
        ++argNum;
    }
    return true;
}

std::string ArgumentParser::getParameter(std::string name) {
    return paramArgs[name];
}

bool ArgumentParser::getFlag(std::string name) {
    return flagArgs[name];
}

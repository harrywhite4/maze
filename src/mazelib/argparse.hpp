#ifndef ARGPARSE_HPP_
#define ARGPARSE_HPP_

#include <map>
#include <string>

class ArgumentParser {
    std::map<std::string, std::string> paramArgs;
    std::map<std::string, bool> flagArgs;

 public:
    void addParamArg(std::string option, std::string def);
    void addFlagArg(std::string option, bool def);

    bool parse(int argc, char *argv[]);

    std::string getParameter(std::string);
    bool getFlag(std::string);
};

#endif  // ARGPARSE_HPP_

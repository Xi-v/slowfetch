// libraries
#include <iostream>
#include <fstream>
#include <string>
#include <pwd.h>
#include <unistd.h>
#include <algorithm>
#include <vector>
#include <format>

// custom
#include "modules.h"
#include "modules_display.h"
#include "disk.h"

// main program
int main() {

    // temp constants
    const std::string BOLD = "\x1b[1m";
    const std::string RESET = "\x1b[0m";


    // loads config for jsonc
    nlohmann::json config = loadConfig("config.jsonc");

    // building modules

    // this whole thing shows how it's laid out
    std::vector<std::string> modules;
    std::vector<std::string> boxNames;

    int maxWidth = 0;
    for (const std::string& name : config["layout"]) {
        if (name == "upperBox" || name == "lowerBox" || name == "separator") {
            modules.push_back("");
            boxNames.push_back(name);
            continue;
        }

        if (name == "getUser") {
            modules.push_back(displayUser(config));
            boxNames.push_back("");
        }
        else if (name == "getOS") {
            modules.push_back(displayOS(config));
            boxNames.push_back("");
        }
        else if (name == "cpuINFO") {
            modules.push_back(displayCPU(config));
            boxNames.push_back("");
        }
        else if (name == "memINFO") {
            modules.push_back(displayMemory(config));
            boxNames.push_back("");
        }
        else if (name == "getDisk") {
            modules.push_back(displayDisk(config, "/"));
            boxNames.push_back("");
            if (isSeparateMount("/home")) {
                modules.push_back(displayDisk(config, "/home"));
                boxNames.push_back("");
            }
        }
        else {
            continue;   // unknown name: skip it, don't read modules.back()
        }

        maxWidth = std::max(maxWidth, visibleWidth(modules.back()));
    }

    int extension = maxWidth - 2;

    for (size_t i = 0; i < modules.size(); i++) {
    if (!boxNames[i].empty()) {
        modules[i] = displayBox(config, boxNames[i], extension);
    }
}

    bool filePresent = false;

    int whitespace = calculateWhitespace(modules.size());
    if (whitespace != -1) {
        filePresent = true;
    }
    else {
        std::cout << "Failed to read file" << std::endl;
    }


    // pulls the data from the config file
    nlohmann::json cfg = config.value("ascii-config", nlohmann::json::object());

    // loops through each line of the ascii.txt file and prints it
    std::ifstream file("ascii.txt");

    // checks if the file opened
    if (filePresent && file.is_open()) {
        std::string line;
        int lineCount = 0;
        int maxWidth = maxWidthCalculator();
        bool bold = false;

        // reads the colour values
        int red = cfg.value("red", 255);
        int green = cfg.value("green", 255);
        int blue = cfg.value("blue", 255);
        int boldInt = cfg.value("bold", 0);

        if (boldInt == 1) {
            bold = true;
        }

        // goes through every line and prints line of ascii
        while (std::getline(file, line)) {

            std::string lineOutput = std::format("\x1b[38;2;{};{};{}m", red, green, blue);

            // checks the config if bold
            if (bold) {
                lineOutput += BOLD;
            }

            lineOutput += line;
            lineOutput += RESET;
            int index = lineCount - whitespace;

            // adds padding the number represents how much empty space
            std::string padding(maxWidth - line.length() + 5, ' ');

            lineOutput += padding;

            if (index >= 0 && index < static_cast<int>(modules.size())) {
                lineOutput += modules[index];
            }

            lineOutput += "\n";

            std::cout << lineOutput;

            lineCount++;
        }
        file.close();
    }

    return 0;
}
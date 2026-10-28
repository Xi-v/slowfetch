// libraries
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <format>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <cstdio>
#include <string>

// custom libraries
#include "modules.h"
#include "modules_display.h"
#include "disk.h"

// constants for features
const std::string BOLD = "\x1b[1m";
const std::string RESET = "\x1b[0m";

// loads jsonc
nlohmann::json loadConfig(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << path << std::endl;
        return nlohmann::json::object();
    }

    // arguments: input, callback, allow_exceptions (false), ignore comments (true)
    nlohmann::json config = nlohmann::json::parse(file, nullptr, false, true);

    if (config.is_discarded()) {
        std::cerr << "Invalid JSON in " << path << std::endl;
        return nlohmann::json::object();
    }

    return config;
}

// EACH OF THE ONES BELOW ARE NOW MODULES

// cpu
std::string displayCPU(const nlohmann::json& config) {
    // gathers and displays CPU information
    std::string cpuModel = cpuINFO();

    // pulls the data from the config file
    nlohmann::json cfg = config.value("cpuINFO-config", nlohmann::json::object());

    // reads the colour values
    int red = cfg.value("red", 255);
    int green = cfg.value("green", 255);
    int blue = cfg.value("blue", 255);

    std::string message = std::format("\x1b[38;2;{};{};{}m", red, green, blue);
    message += "CPU \uf4bc:";
    message += cpuModel;
    message += RESET;

    return message;
}

// user
std::string displayUser(const nlohmann::json& config) {
    // gathers user
    std::string user = getUser();

    // pulls the data from the config file
    nlohmann::json cfg = config.value("getUser-config", nlohmann::json::object());

    // reads the colour values
    int red = cfg.value("red", 255);
    int green = cfg.value("green", 255);
    int blue = cfg.value("blue", 255);

    std::string message = std::format("\x1b[38;2;{};{};{}m", red, green, blue);
    message += BOLD;
    message += user;
    message += RESET;

    return message;
}

// os
std::string displayOS(const nlohmann::json& config) {
    // gathers os
    std::string os = getOS();

    // pulls the data from the config file
    nlohmann::json cfg = config.value("getOS-config", nlohmann::json::object());   

    // reads the colour values
    int red = cfg.value("red", 255);
    int green = cfg.value("green", 255);
    int blue = cfg.value("blue", 255);

    std::string message = std::format("\x1b[38;2;{};{};{}m", red, green, blue);
    message += "OS \uf121: ";
    message += os;
    message += RESET;

    return message;
}

// memory
std::string displayMemory(const nlohmann::json& config) {
    // gathers memory info
    MemoryInfo memory = memINFO();

    // pulls the data from the config file
    nlohmann::json cfg = config.value("memINFO-config", nlohmann::json::object());   

    // reads the colour values
    int red = cfg.value("red", 255);
    int green = cfg.value("green", 255);
    int blue = cfg.value("blue", 255);

    std::string message= std::format("\x1b[38;2;{};{};{}m", red, green, blue);
    message += std::format("Memory \uefc5: {:.2f} GiB / {:.2f} GiB", memory.used, memory.total); // works when built dw  bro 
    message += RESET;

    return message;
}

// logic for all boxes
std::string displayBox(const nlohmann::json& config, const std::string& name, int autoExtension) {
    nlohmann::json cfg = config.value(name + "-config", nlohmann::json::object());

    // -1 in the config means it's gotta work it out
    int length = cfg.value("length", -1);
    int extension = (length == -1) ? autoExtension : length;

    // reads the colour values
    int red = cfg.value("red", 255);
    int green = cfg.value("green", 255);
    int blue = cfg.value("blue", 255);

    std::string startChar = cfg.value("startChar", "|");
    std::string endChar = cfg.value("endChar", "|");
    std::string pipeChar = cfg.value("pipeChar", "=");

    std::string message = std::format("\x1b[38;2;{};{};{}m", red, green, blue);
    message += startChar;
    for (int i = 0; i < extension; i++) {
        message += pipeChar;
    }
    message += endChar;
    message += RESET;

    return message;
}

int visibleWidth(const std::string& s) {
    int width = 0;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '\x1b') {
            while (i < s.size() && s[i] != 'm') i++;
        }
        else if ((s[i] & 0xC0) != 0x80) {
            width++;
        }
    }
    return width;
}

// disk
std::string displayDisk(const nlohmann::json& config, const char* path) {
    // gathers
    std::string disk = formatDisk(path);
 
    // pulls the data from the config file
    nlohmann::json cfg = config.value("getDisk-config", nlohmann::json::object());

    // reads the colour values
    int red = cfg.value("red", 255);
    int green = cfg.value("green", 255);
    int blue = cfg.value("blue", 255);

    std::string message = std::format("\x1b[38;2;{};{};{}m", red, green, blue);
    message += disk;
    message += RESET;

    return message;
}

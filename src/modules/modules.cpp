// libraries
#include "modules.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <format>
#include <iomanip>
#include <pwd.h>
#include <unistd.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <cstdio>

// read cpu info
std::string cpuINFO() {
    // contains cpu model
    std::ifstream inputFile("/proc/cpuinfo");
    std::string line;
    std::string target = "model name";
    bool found = false;

    // final output
    std::string cpuModel;

    // reads through every line in the file
    while (std::getline(inputFile, line) && !found) {
        if (line.find(target) != std::string::npos) {
            // prepares the line to be split
            std::stringstream ss(line);
            std::string token;
            int tokenIndex = 0;

            // splits the line at the colon and then only takes the second string
            while (getline(ss, token, ':')) {
                if (tokenIndex == 1) {
                    cpuModel = token;
                    break;
                }
                tokenIndex++;
            }

            found = true;
        }
    }

    return cpuModel;
}

// gets user@hostname
std::string getUser() {

    passwd* user = getpwuid(getuid());

    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        std::cerr << "Could not get hostname" << std::endl;
        
    }
    if (user == nullptr) {
        std::cerr << "Could not get username" << std::endl;
    }

    std::string message = user->pw_name;
    message += '@';
    message +=hostname;

    return message;
}

// gets the distro that is being used
std::string getOS() {
    // contains os
    std::ifstream inputFile("/etc/os-release");
    std::string line;
    std::string target = "PRETTY_NAME";

    // final output
    std::string os;

    // reads through every line in the file
    while (std::getline(inputFile, line)) {
        if (line.find(target) != std::string::npos) {
            // prepares the line to be split
            std::stringstream ss(line);
            std::string token;
            int tokenIndex = 0;

            // splits the line at the colon and then only takes the second string
            while (getline(ss, token, '=')) {
                if (tokenIndex == 1) {
                    os = token;
                    break;
                }
                tokenIndex++;
            }
        }
    }

    // 1 liner that removes every instance of a character in a string
    os.erase(std::remove(os.begin(), os.end(), '"'), os.end());

    return os;
}

// calculate whitespace
int calculateWhitespace(int modules) {
    int whitespace;
    std::ifstream file("ascii.txt");

    // checks if file is there 
    if (file.is_open()) {
        std::string line;
        int lineCount = 0;

        // adds 1 for every line in file
        while (std::getline(file, line)) {
            lineCount++;
        }

        file.close();

        whitespace = floor((lineCount - modules) / 2.0);
    }
    else {
        whitespace = -1;
    }
    return whitespace;
}

// puts each line of the file into a vector
int maxWidthCalculator() {
    std::vector<std::string> file;
    std::ifstream asciiFile("ascii.txt");

    int maxWidth = 0;
    int lineWidth;

    if (asciiFile.is_open()) {
        std::string line;


        while (std::getline(asciiFile, line)) {
            lineWidth = line.length();

            if (lineWidth > maxWidth) {
                maxWidth = lineWidth;
            }

            file.push_back(line);
        }

        asciiFile.close();
    }

    return maxWidth;
}

// reads memory info
MemoryInfo memINFO() {
    // contains memory information
    std::ifstream inputFile("/proc/meminfo");
    std::string line;

    // needs 2 targets because we reading 2 values
    std::string target = "MemTotal";
    std::string target1 = "MemAvailable";

    // final output 
    std::string totalMemory;
    std::string usedMemory;
    std::string formattedMemory;

    // reads through every line in the file for MemTotal and MemFree
    while (std::getline(inputFile, line)) {
        // memTotal finder
        if (line.find(target) != std::string::npos) {
            // prepares the line to be split
            std::stringstream ss(line);
            std::string token;
            int tokenIndex = 0;

            // splits the line at the colon and then only takes the second string
            while (getline(ss, token, ':')) {
                if (tokenIndex == 1) {
                    totalMemory = token;
                    break;
                }
                tokenIndex++;
            }
        }
        // usedMemory finder
        else if (line.find(target1) != std::string::npos) {
            // prepares the line to be split
            std::stringstream ss(line);
            std::string token;
            int tokenIndex = 0;

            // splits the line at the colon and then only takes the second string
            while (getline(ss, token, ':')) {
                if (tokenIndex == 1) {
                    usedMemory = token;
                    break;
                }
                tokenIndex++;
            }
        }
    }

    // split total memory
    std::stringstream ss(totalMemory);
    std:: string token;
    std::string totalMemoryInteger;

    // splits the line at the colon and then only takes the second string
    while (getline(ss, token, ' ')) {
        if (!token.empty()) {
            totalMemoryInteger = token;
            break;
        }
    }

    // split used memory
    std::stringstream ss1(usedMemory);
    std:: string token1;
    std::string usedMemoryInteger;

    // splits the line at the colon and then only takes the second string
    while (getline(ss1, token1, ' ')) {
        if (!token1.empty()) {
            usedMemoryInteger = token1;
            break;
        }
    }

    // coonverts them from kibibytes to gibibytes
    double totalMem = std::stod(totalMemoryInteger);
    double usedMem = std::stod(usedMemoryInteger);

    totalMem = totalMem / (1024 * 1024);
    usedMem = usedMem / (1024 * 1024);
    usedMem = totalMem - usedMem;

    return {totalMem, usedMem};

}
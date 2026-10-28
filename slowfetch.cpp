// libraries
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

// struct for reading mem info
struct MemoryInfo {
    double total;
    double used;
};

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
  

// main program
int main() {
    // fancy things
    const std::string BOLD = "\x1b[1m";
    const std::string PASTEL_PINK = "\x1b[38;2;255;204;255m";
    const std::string PASTEL_YELLOW = "\x1b[38;2;255;255;204m";
    const std::string PASTEL_BLUE = "\x1b[38;2;204;255;255m";
    const std::string PASTEL_PURPLE = "\x1b[38;2;204;204;255m";
    const std::string PASTEL_GREEN = "\x1b[38;2;204;255;204m";
    const std::string RESET = "\x1b[0m";

    // building modules

    int moduleMaxWidth = 0;
    int lineWidth;
    int totalSpaces = 0;
    int maxSpaces = 0;

    // gathers user and hostname
    std::string user = getUser();
    // std::cout << BOLD << PASTEL_PINK << user << RESET << std::endl;
    std::string getUser = BOLD;
    getUser += PASTEL_PINK;
    getUser += user;
    getUser += RESET;

    lineWidth = getUser.length();

    if (lineWidth > moduleMaxWidth) {
        moduleMaxWidth = lineWidth;
    }

    totalSpaces = std::count(getUser.begin(), getUser.end(), ' ');

    if (totalSpaces > maxSpaces) {
        maxSpaces = totalSpaces;
    }



    // gathers os
    std::string os = getOS();
    // std::cout << PASTEL_YELLOW << formattedOS << RESET << std::endl;
    std::string getOS = PASTEL_YELLOW;
    getOS += "OS \uf121: ";
    getOS += os;
    getOS += RESET;

    lineWidth = getUser.length();

    if (lineWidth > moduleMaxWidth) {
        moduleMaxWidth = lineWidth;
    }

    totalSpaces = std::count(getOS.begin(), getOS.end(), ' ');

    if (totalSpaces > maxSpaces) {
        maxSpaces = totalSpaces;
    }

    // gathers and displays CPU information
    std::string cpuModel = cpuINFO();
    // std::cout << PASTEL_BLUE << formattedCpu << RESET << std::endl;
    std::string getCPU = PASTEL_BLUE;
    getCPU += "CPU Model \uf4bc:";
    getCPU += cpuModel;
    getCPU += RESET;

    lineWidth = getUser.length();

    if (lineWidth > moduleMaxWidth) {
        moduleMaxWidth = lineWidth;
    }

    totalSpaces = std::count(getCPU.begin(), getCPU.end(), ' ');

    if (totalSpaces > maxSpaces) {
        maxSpaces = totalSpaces;
    }

    // gathers and displays memory information
    MemoryInfo memory = memINFO();
    // std::cout << std::fixed << std::setprecision(2) << PASTEL_PURPLE << "Memory: " << memory.used << " GiB" << " / " << memory.total << " GiB" << RESET << std::endl;
    std::string getMemory = PASTEL_PURPLE;
    getMemory += std::format("Memory \uefc5: {:.2f} GiB / {:.2f} GiB", memory.used, memory.total); // works when built dw  bro 
    getMemory += RESET;

    lineWidth = getUser.length();

    if (lineWidth > moduleMaxWidth) {
        moduleMaxWidth = lineWidth;
    }

    totalSpaces = std::count(getMemory.begin(), getMemory.end(), ' ');

    if (totalSpaces > maxSpaces) {
        maxSpaces = totalSpaces;
    }

    // how far out the pipes extend
    int extension = moduleMaxWidth + maxSpaces - 2;

    // upper box
    std::string upperBox = "┌";
    // like the command '-'*int in python
    for (int i = 0; i < extension; i++) {
        upperBox += "=";
    }
    upperBox += "┐";

    // lower box
    std::string lowerBox = "└";
    // like the command '-'*int in python
    for (int i = 0; i < extension; i++) {
        lowerBox += "=";
    }
    lowerBox += "┘";

    // user separator
    std::string userSeparator = "|";
    for (int i = 0; i < extension; i++) {
        userSeparator += "-";
    }
    userSeparator += "|";

    std::vector<std::string> modules = {upperBox, getUser, userSeparator, getOS, getCPU, getMemory, lowerBox};

    bool filePresent = false;

    int whitespace = calculateWhitespace(modules.size());
    if (whitespace != -1) {
        filePresent = true;
    }
    else {
        std::cout << "Failed to read file" << std::endl;
    }

    // loops through each line of the ascii.txt file and prints it
    std::ifstream file("ascii.txt");

    // checks if the file opened
    if (filePresent && file.is_open()) {
        std::string line;
        int lineCount = 0;
        int maxWidth = maxWidthCalculator();

        // goes through every line and prints line of ascii
        while (std::getline(file, line)) {
            std::string lineOutput;

            lineOutput += BOLD;
            lineOutput += PASTEL_GREEN;
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
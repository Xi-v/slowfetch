#ifndef MODULES
#define MODULES

#include<string>

// struct for memory
struct MemoryInfo {
    double total;
    double used;
};

// all the different functions for the program
std::string cpuINFO();
std::string getUser();
std::string getOS();
int calculateWhitespace(int modules);
int maxWidthCalculator();
MemoryInfo memINFO();

#endif
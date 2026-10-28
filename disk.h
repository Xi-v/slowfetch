#ifndef DISK
#define DISK
#include <string>

// struct for Disk
struct DiskInfo {
    unsigned long long total;
    unsigned long long used;
};


std::string formatDisk(const char* path);
bool isSeparateMount(const char* path);

#endif
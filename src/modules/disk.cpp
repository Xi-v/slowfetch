#include "disk.h"

#include <sys/statvfs.h>
#include <sys/stat.h>
#include <cstdio>
#include <string>


bool getDisk(const char* path, DiskInfo& out) {
    struct statvfs s;
    if (statvfs(path, &s) != 0) {
        return false;
    }
    out.total = (unsigned long long)s.f_blocks * s.f_frsize;
    out.used  = (unsigned long long)(s.f_blocks - s.f_bfree) * s.f_frsize;
    return true;
}

bool isSeparateMount(const char* path) {
    struct stat root, other;
    if (stat("/", &root) != 0 || stat(path, &other) != 0) {
        return false;
    }
    return root.st_dev != other.st_dev;
}

std::string gib(unsigned long long bytes) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%.2f GiB", bytes / (1024.0 * 1024.0 * 1024.0));
    return buf;
}

std::string formatDisk(const char* path) {
    DiskInfo d;
    if (!getDisk(path, d)) {
        return "";
    }

    std::string message = "Disk (";
    message += path;
    message += "): ";
    message += gib(d.used);
    message += " / ";
    message += gib(d.total);
    return message;
}

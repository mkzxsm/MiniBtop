#include "SystemInfoReader.hpp"
#include <fstream>
#include <unistd.h>
#include <limits.h>

SysInfo SystemInfoReader::getInfo() {
    SysInfo info;

    char host[HOST_NAME_MAX];
    if (gethostname(host, HOST_NAME_MAX) == 0) info.hostname = host;

    char* user = getlogin();
    info.username = user ? user : "user";

    std::ifstream uptimeFile("/proc/uptime");
    if (uptimeFile.is_open()) {
        double uptimeSeconds;
        uptimeFile >> uptimeSeconds;
        int hours = static_cast<int>(uptimeSeconds) / 3600;
        int minutes = (static_cast<int>(uptimeSeconds) % 3600) / 60;
        info.uptime = std::to_string(hours) + "h " + std::to_string(minutes) + "m";
    }
    return info;
}
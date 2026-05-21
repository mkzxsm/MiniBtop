#pragma once
#include <string>

struct SysInfo {
    std::string username;
    std::string hostname;
    std::string uptime;
};

class SystemInfoReader {
public:
    static SysInfo getInfo();
};
#include "MemoryReader.hpp"
#include <fstream>
#include <sstream>
#include <string>

MemStats MemoryReader::getStats() {
    std::ifstream file("/proc/meminfo");
    std::string line, key;
    long long val;
    std::string unit;
    MemStats stats = {0, 0, 0, 0.0f};

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        ss >> key >> val >> unit;

        if (key == "MemTotal:") stats.total = val / 1024;
        if (key == "MemAvailable:") stats.available = val / 1024;
    }

    stats.used = stats.total - stats.available;
    if (stats.total > 0) {
        stats.usagePercentage = static_cast<float>(stats.used) / stats.total;
    }
    return stats;
}
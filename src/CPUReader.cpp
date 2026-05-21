#include "CPUReader.hpp"
#include <fstream>
#include <sstream>
#include <string>

float CPUReader::getUsage() {
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);

    std::stringstream ss(line);
    std::string cpuLabel;
    long long user, nice, system, idle, iowait, irq, softirq, steal;

    ss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    long long totalIdle = idle + iowait;
    long long nonIdle = user + nice + system + irq + softirq + steal;
    long long total = totalIdle + nonIdle;

    long long totalDiff = total - prevTotal;
    long long idleDiff = totalIdle - prevIdle;

    float cpuUsage = 0.0f;
    if (totalDiff != 0) {
        cpuUsage = static_cast<float>(totalDiff - idleDiff) / totalDiff;
    }

    prevTotal = total;
    prevIdle = totalIdle;

    return cpuUsage;
}
#include "NetworkReader.hpp"
#include <fstream>
#include <sstream>
#include <string>

NetStats NetworkReader::getSpeed() {
    std::ifstream file("/proc/net/dev");
    std::string line;
    long long totalRx = 0;
    long long totalTx = 0;

    std::getline(file, line);
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string iface;
        ss >> iface;
        if (iface.find("lo:") != std::string::npos) continue;

        long long rx, tx, dummy;
        ss >> rx >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> tx;
        totalRx += rx;
        totalTx += tx;
    }

    NetStats stats = {0.0, 0.0};
    if (prevRx != 0 || prevTx != 0) {
        stats.rxSpeedMB = static_cast<double>(totalRx - prevRx) / (1024.0 * 1024.0);
        stats.txSpeedMB = static_cast<double>(totalTx - prevTx) / (1024.0 * 1024.0);
    }

    prevRx = totalRx;
    prevTx = totalTx;
    return stats;
}
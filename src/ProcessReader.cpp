#include "ProcessReader.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <unistd.h>

namespace fs = std::filesystem;

std::vector<ProcessInfo> ProcessReader::getTopMemoryProcesses(int count) {
    std::vector<ProcessInfo> processes;

    long pageSizeKB = sysconf(_SC_PAGESIZE) / 1024;

    for (const auto& entry : fs::directory_iterator("/proc")) {
        std::string dirname = entry.path().filename().string();
        if (!std::all_of(dirname.begin(), dirname.end(), ::isdigit)) {
            continue;
        }

        int pid = std::stoi(dirname);
        std::string name = "";
        long memoryMB = 0;

        std::ifstream commFile(entry.path() / "comm");
        if (commFile.is_open()) {
            std::getline(commFile, name);
        }

        std::ifstream statmFile(entry.path() / "statm");
        if (statmFile.is_open()) {
            long size, resident;
            statmFile >> size >> resident;
            memoryMB = (resident * pageSizeKB) / 1024;
        }

        if (!name.empty() && memoryMB > 0) {
            processes.push_back({pid, name, memoryMB});
        }
    }

    std::sort(processes.begin(), processes.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.memoryMB > b.memoryMB;
    });

    if (processes.size() > static_cast<size_t>(count)) {
        processes.resize(count);
    }

    return processes;
}
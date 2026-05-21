#pragma once
#include <string>
#include <vector>

struct ProcessInfo {
    int pid;
    std::string name;
    long memoryMB;
};

class ProcessReader {
public:
    static std::vector<ProcessInfo> getTopMemoryProcesses(int count = 10);
};
#pragma once
#include "CPUReader.hpp"
#include "NetworkReader.hpp"

class MiniBtopApp {
private:
    CPUReader cpuReader;
    NetworkReader netReader;
    bool isRunning;

public:
    MiniBtopApp();
    void run();
};
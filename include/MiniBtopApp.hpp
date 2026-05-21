#pragma once
#include "CPUReader.hpp"

class MiniBtopApp {
private:
    CPUReader cpuReader;
    bool isRunning;

public:
    MiniBtopApp();
    void run();
};
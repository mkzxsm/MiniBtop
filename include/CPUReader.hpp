#pragma once

class CPUReader {
private:
    long long prevTotal = 0;
    long long prevIdle = 0;

public:
    float getUsage();
};
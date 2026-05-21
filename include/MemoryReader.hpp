#pragma once

struct MemStats {
    long long total;
    long long available;
    long long used;
    float usagePercentage;
};

class MemoryReader {
public:
    static MemStats getStats();
};
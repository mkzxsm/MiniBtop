#pragma once

struct NetStats {
    double rxSpeedMB;
    double txSpeedMB;
};

class NetworkReader {
private:
    long long prevRx = 0;
    long long prevTx = 0;
public:
    NetStats getSpeed();
};
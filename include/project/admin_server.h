#pragma once
#include "metrics.h"

class AdminServer {
public:
    AdminServer(int port, Metrics* metrics);
    void start();

private:
    int port_;
    Metrics* metrics_;
};

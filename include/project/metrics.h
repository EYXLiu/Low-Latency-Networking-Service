#pragma once
#include <atomic>

class Metrics {
public:
    Metrics() = default;

    void increment_connections();
    void decrement_connections();
    int get_connections();

private:
    std::atomic<int> active_connections_{0};
};
#pragma once
#include <atomic>

class Metrics {
public:
    void increment_connections();
    void decrement_connections();
    void increment_messages();
    int get_connections();

private:
    std::atomic<int> active_connections_{0};
    std::atomic<int> total_messages_{0};
};
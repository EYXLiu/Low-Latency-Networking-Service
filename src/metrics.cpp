#include "project/metrics.h"

void Metrics::increment_connections() {
    active_connections_++;
}

void Metrics::decrement_connections() {
    active_connections_--;
}

int Metrics::get_connections() {
    return active_connections_.load();
}

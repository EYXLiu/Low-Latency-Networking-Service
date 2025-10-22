#pragma once
#include <unordered_map>
#include <memory>
#include <mutex>
#include "connection.h"
#include "metrics.h"

class ConnectionManager {
public:
    ConnectionManager();
    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    void add_connection(std::unique_ptr<Connection> conn);
    void remove_connection(int fd);
    Connection* get_connection(int fd);
    std::vector<Connection*> get_all_connections();
    int get_connections();

private:
    std::unordered_map<int, std::unique_ptr<Connection>> connections_;
    std::mutex mtx_;
    Metrics metrics_;
};

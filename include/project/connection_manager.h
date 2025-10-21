#pragma once
#include <unordered_map>
#include <memory>
#include <mutex>
#include "connection.h"

class ConnectionManager {
public:
    ConnectionManager() = default;
    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    void add_connection(std::unique_ptr<Connection> conn);
    void remove_connection(int fd);
    Connection* get_connection(int fd);
    std::vector<Connection*> get_all_connections();

private:
    std::unordered_map<int, std::unique_ptr<Connection>> connections_;
    std::mutex mtx_;
};

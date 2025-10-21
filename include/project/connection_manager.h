#pragma once
#include <unordered_map>
#include <memory>
#include <mutex>
#include "connection.h"

class ConnectionManager {
public:
    void add_connection(std::unique_ptr<Connection> conn);
    void remove_connection(int fd);
    Connection* get_connection(int fd);

private:
    std::unordered_map<int, std::unique_ptr<Connection>> connections_;
    std::mutex mtx_;
};

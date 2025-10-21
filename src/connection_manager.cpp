#include "project/connection_manager.h"

void ConnectionManager::add_connection(std::unique_ptr<Connection> conn) {
    std::lock_guard<std::mutex> lock(mtx_);
    int fd = conn->get_fd();
    connections_[fd] = std::move(conn);
}

void ConnectionManager::remove_connection(int fd) {
    std::lock_guard<std::mutex> lock(mtx_);
    connections_.erase(fd);
}

Connection* ConnectionManager::get_connection(int fd) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = connections_.find(fd);
    return it != connections_.end() ? it->second.get() : nullptr;
}

std::vector<Connection*> ConnectionManager::get_all_connections() {
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<Connection*> conns;
    for (auto& [fd, conn_ptr] : connections_) {
        conns.push_back(conn_ptr.get());
    }
    return conns;
}

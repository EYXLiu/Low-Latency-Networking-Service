#pragma once
#include <netinet/in.h>
#include <memory>

class Reactor;
class ConnectionManager;
class Metrics;

class Acceptor {
public:
    Acceptor(int port, Reactor& reactor, ConnectionManager& conn_mgr, std::atomic<bool>& running);
    ~Acceptor();

    void start_accepting();

private:
    int listen_fd_;
    int port_;
    
    Reactor& reactor_;
    ConnectionManager& conn_mgr_;

    std::atomic<bool>& running_;

    int setup_listener();
    void handle_new_connection();
};

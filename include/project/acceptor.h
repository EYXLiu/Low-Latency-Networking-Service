#pragma once
#include <netinet/in.h>
#include <memory>

class Reactor;
class ConnectionManager;

class Acceptor {
public:
    Acceptor(int port, Reactor& reactor, ConnectionManager& conn_mgr);
    ~Acceptor();

    void start_accepting();

private:
    int listen_fd_;
    int port_;
    Reactor& reactor_;
    ConnectionManager& conn_mgr_;

    int setup_listener();
    void handle_new_connection();
};

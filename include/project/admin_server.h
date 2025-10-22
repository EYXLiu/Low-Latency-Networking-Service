#pragma once
#include "metrics.h"
#include "reactor.h"
#include "acceptor.h"
#include "connection_manager.h"
#include <thread>
#include <iostream>
#include <atomic>

class AdminServer {
public:
    AdminServer(int port);
    ~AdminServer();
    void start();
    void stop();

private:
    int http_port_;
    int listen_fd_;

    ConnectionManager conn_mgr_;
    Reactor reactor_;
    Acceptor acceptor_;
    std::thread thread_;
    std::thread http_thread_;

    std::atomic<bool> running_{false};

    void run_loop();
    void handle_http_request(int client_fd);
};

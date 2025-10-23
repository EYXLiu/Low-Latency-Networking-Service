#include "project/acceptor.h"
#include "project/connection_manager.h"
#include "project/reactor.h"
#include "project/connection.h"
#include "project/metrics.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

Acceptor::Acceptor(int port, Reactor& reactor, ConnectionManager& conn_mgr, std::atomic<bool>& running) : listen_fd_(-1), port_(port), reactor_(reactor), conn_mgr_(conn_mgr), running_(running) {};

Acceptor::~Acceptor() { if (listen_fd_ >= 0) close(listen_fd_); }

int Acceptor::setup_listener() {
    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd_ < 0) return -1;

    int opt = 1;
    if (setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) return -2;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);

    if (bind(listen_fd_, (sockaddr*)&addr, sizeof(addr)) < 0) return -2;
    if (listen(listen_fd_, SOMAXCONN) < 0) return -3;

    fcntl(listen_fd_, F_SETFL, fcntl(listen_fd_, F_GETFL, 0) | O_NONBLOCK);
    return 0;
}

void Acceptor::handle_new_connection() {
    while (running_) {
        sockaddr_in client_addr{};
        socklen_t addrlen = sizeof(client_addr);
        int client_fd = accept(listen_fd_, (sockaddr*)&client_addr, &addrlen);

        if (client_fd < 0) {
            break;
        }

        int flags = fcntl(client_fd, F_GETFL, 0);
        fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

        auto conn = std::make_unique<Connection>(client_fd);
        Connection* conn_ptr = conn.get();
        
        conn_mgr_.add_connection(std::move(conn));
        reactor_.add_fd(client_fd, true, true, conn_ptr);
    }
}

void Acceptor::start_accepting() {
    if (setup_listener() != 0) {
        std::cerr << "failed to set up listener" << std::endl;
        return;
    }

    while (running_) {
        handle_new_connection();
        usleep(1000);
    }
}

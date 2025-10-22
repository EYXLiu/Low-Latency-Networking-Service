#include "project/admin_server.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>

AdminServer::AdminServer(int port) : http_port_(8080), reactor_(conn_mgr_), acceptor_(port, reactor_, conn_mgr_) {}

AdminServer::~AdminServer() { stop(); }

void AdminServer::start() {
    running_ = true;

    thread_ = std::thread([this]() {
        acceptor_.start_accepting();
        reactor_.run_loop();
    });

    http_thread_ = std::thread([this]() {
        run_loop();
    });
}

void AdminServer::stop() {
    running_ = false;
    if (thread_.joinable()) thread_.join();
    if (http_thread_.joinable()) http_thread_.join();
}

void AdminServer::run_loop() {
    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(http_port_);

    bind(listen_fd_, (struct sockaddr*)&addr, sizeof(addr));
    listen(listen_fd_, SOMAXCONN);

    std::cout << "Admin server listening on port " << http_port_ << std::endl;

    while (running_) {
        int client_fd = accept(listen_fd_, nullptr, nullptr);
        if (client_fd < 0) continue;

        std::thread([this, client_fd]() {
            handle_http_request(client_fd);
        }).detach();
    }
}

void AdminServer::handle_http_request(int client_fd) {
    char buffer[1024];
    ssize_t n = read(client_fd, buffer, sizeof(buffer));
    if (n <= 0) { close(client_fd); return; }

    std::string request(buffer, n);
    std::string body;

    if (request.find("GET /metrics") != std::string::npos) {
        body = "{ \"connections\": " + std::to_string(conn_mgr_.get_connections()) + "}";
    } else {
        body = "Unknown endpoint";
    }

    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " +
                           std::to_string(body.size()) + "\r\n\r\n" +
                           body;

    write(client_fd, response.c_str(), response.size());
    close(client_fd);
}

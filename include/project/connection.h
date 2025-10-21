#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include "message.h"

enum class ConnectionState {
    CONNECTED,
    AUTHENTICATED,
    CLOSED
};

class Connection {
public:
    Connection(int fd);
    ~Connection();

    void on_readable();
    void on_writable();
    void send_message(std::unique_ptr<Message> msg);

    int get_fd() const { return fd_; }

private:
    int fd_;
    ConnectionState state_;
    std::vector<uint8_t> read_buffer_;
    std::vector<uint8_t> write_buffer_;
    bool backpressured_;

    void flush_write_buffer();
};

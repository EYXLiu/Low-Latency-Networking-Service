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

class Reactor;

class Connection {
public:
    Connection(int fd);
    ~Connection();

    int on_readable();
    void on_writable();
    void send_message(std::unique_ptr<Message> msg);
    void flush_write_buffer();
    bool write_buffer_empty();

    int get_fd() const { return fd_; }
    std::vector<uint8_t>& get_read_buffer() { return read_buffer_; }

private:
    int fd_;
    //ConnectionState state_;
    std::vector<uint8_t> read_buffer_;
    std::vector<uint8_t> write_buffer_;
    bool backpressured_;
};

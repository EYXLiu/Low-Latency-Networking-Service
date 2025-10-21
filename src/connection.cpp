#include "project/connection.h"
#include "project/parser.h"
#include "project/reactor.h"
#include <unistd.h>
#include <sys/uio.h>
#include <iostream>

Connection::Connection(int fd) : fd_(fd), backpressured_(false) {}
Connection::~Connection() { close(fd_); }

int Connection::on_readable() {
    uint8_t buf[4096];
    ssize_t n = read(fd_, buf, sizeof(buf));
    if (n <= 0) {
        std::cout << "Client closed connection fd=" << fd_ << std::endl;
        close(fd_);
        return 1;
    }
    read_buffer_.insert(read_buffer_.end(), buf, buf + n);

    std::cout << "Received from fd " << fd_ << ": ";
    for (int i = 0; i < n; ++i) {
        printf("%02X ", buf[i]);
    }
    std::cout << std::endl;

    return 0;
}

void Connection::on_writable() { 
    std::cout << "Sending to fd " << fd_ << std::endl;
    flush_write_buffer(); 
}

void Connection::send_message(std::unique_ptr<Message> msg) {
    auto bytes = msg->serialize();
    write_buffer_.insert(write_buffer_.end(), bytes.begin(), bytes.end());
    if (write_buffer_.size() > 64 * 1024)
        backpressured_ = true;
}

void Connection::flush_write_buffer() {
    if (write_buffer_.empty()) return;
    std::cout << "Sending " << write_buffer_.size() << " bytes on fd " << fd_ << ": ";
    for (auto b : write_buffer_) {
        printf("%02X ", b);
    }
    std::cout << std::endl;
    ssize_t n = write(fd_, write_buffer_.data(), write_buffer_.size());
    if (n > 0) write_buffer_.erase(write_buffer_.begin(), write_buffer_.begin() + n);
}

bool Connection::write_buffer_empty() {
    return write_buffer_.empty();
}

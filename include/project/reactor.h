#pragma once
#include <sys/types.h>
#include <sys/event.h>
#include <vector>
#include <unordered_map>
#include <memory>

class Connection;

class Reactor {
public:
    Reactor();
    ~Reactor();

    void add_fd(int fd, bool want_read, bool want_write);
    void remove_fd(int fd);
    void run_loop();

private:
    int kqueue_fd_;
    struct kevent events_[1024];

    std::unordered_map<int, Connection*> fd_to_conn_;
    void handle_event(struct kevent& ev);
    void handle_read(Connection* conn);
    void handle_write(Connection* conn);
};

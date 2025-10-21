#pragma once
#include <sys/types.h>
#include <sys/event.h>
#include <vector>
#include <unordered_map>
#include <memory>

class Connection;
class ConnectionManager;
struct Message;

class Reactor {
public:
    Reactor(ConnectionManager& conn_mgr);
    ~Reactor();

    void add_fd(int fd, bool want_read, bool want_write, Connection* conn_ptr);
    void remove_fd(int fd);
    void run_loop();
    void broadcast_message(Connection* sender, std::unique_ptr<Message> msg);

private:
    int kqueue_fd_;
    struct kevent events_[1024];
    ConnectionManager& conn_mgr_;

    std::unordered_map<int, Connection*> fd_to_conn_;
    void handle_event(struct kevent& ev);
    void handle_read(Connection* conn);
    void handle_write(Connection* conn);
};

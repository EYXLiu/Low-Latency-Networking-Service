#include "project/reactor.h"
#include "project/connection.h"
#include "project/connection_manager.h"
#include "project/write_coalescer.h"
#include "project/parser.h"
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <errno.h>

Reactor::Reactor(ConnectionManager& conn_mgr) : conn_mgr_(conn_mgr) {
    kqueue_fd_ = kqueue();
    if (kqueue_fd_ < 0) {
        exit(1);
    }
}

Reactor::~Reactor() {
    close(kqueue_fd_);
}

void Reactor::add_fd(int fd, bool want_read, bool want_write, Connection* conn_ptr) {
    struct kevent ev_set[2];
    int n = 0;

    if (want_read) {
        EV_SET(&ev_set[n++], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, nullptr);
    }
    if (want_write) {
        EV_SET(&ev_set[n++], fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, nullptr);
    }
    
    fd_to_conn_[fd] = conn_ptr;
    kevent(kqueue_fd_, ev_set, n, nullptr, 0, nullptr);
}

void Reactor::remove_fd(int fd) {
    struct kevent ev_set[2];
    EV_SET(&ev_set[0], fd, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
    EV_SET(&ev_set[1], fd, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);
    kevent(kqueue_fd_, ev_set, 2, nullptr, 0, nullptr);

    fd_to_conn_.erase(fd);
}

void Reactor::run_loop() {
    while (true) {
        int nev = kevent(kqueue_fd_, nullptr, 0, events_, 1024, nullptr);
        if (nev < 0) {
            perror("kevent wait");
            continue;
        }

        for (int i = 0; i < nev; i++) {
            handle_event(events_[i]);
        }
    }
}

void Reactor::handle_event(struct kevent& ev) {
    int fd = ev.ident;
    auto it = fd_to_conn_.find(fd);
    if (it == fd_to_conn_.end()) {
        return;
    }

    Connection* conn = it->second;

    if (ev.filter == EVFILT_READ) {
        handle_read(conn);
    } else if (ev.filter == EVFILT_WRITE) {
        handle_write(conn);
    } else {
        std::cerr << "Unknown event filter"  << std::endl;;
    }
}

void Reactor::handle_read(Connection* conn) {
    if (!conn) return;
    if (conn->on_readable() == 1) {
        remove_fd(conn->get_fd());
        conn_mgr_.remove_connection(conn->get_fd());
    };
    while (auto msg_opt = Parser::try_parse(*conn)) {
        for (auto [fd, c] : fd_to_conn_) {
            if (c != conn) {
                auto& m = *msg_opt;
                WriteCoalescer::queue_message(c, m->clone());
                struct kevent ev;
                EV_SET(&ev, c->get_fd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, nullptr);
                kevent(kqueue_fd_, &ev, 1, nullptr, 0, nullptr);
            }
        }
    }
}

void Reactor::handle_write(Connection* conn) {
    if (!conn) return;
    conn->on_writable();
    if (conn->write_buffer_empty()) {
        struct kevent ev;
        EV_SET(&ev, conn->get_fd(), EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);
        kevent(kqueue_fd_, &ev, 1, nullptr, 0, nullptr);
    }
}

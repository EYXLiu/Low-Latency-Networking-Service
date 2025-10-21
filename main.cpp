#include "project/reactor.h"
#include "project/acceptor.h"
#include "project/connection_manager.h"
#include <thread>
#include <iostream>

int main() {
    ConnectionManager conn_mgr;
    Reactor reactor(conn_mgr);
    Acceptor acceptor(5555, reactor, conn_mgr);

    std::thread accept_thread([&acceptor]() {
        acceptor.start_accepting();
    });

    reactor.run_loop();

    accept_thread.join();
    return 0;
}
#include "project/admin_server.h"
#include <csignal>
#include <atomic>
#include <iostream>
#include <thread>
#include <chrono>

std::atomic<bool> g_running{true};

void signal_handler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nSIGINT received, stopping server..." << std::endl;
        g_running = false;
    }
}

int main() {
    std::signal(SIGINT, signal_handler);

    AdminServer admin(5555);
    admin.start();

    while (g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Server fully stopped. Exiting main." << std::endl;
    admin.stop();
    return 0;
}
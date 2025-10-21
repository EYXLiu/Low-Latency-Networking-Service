#pragma once
#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

class ThreadPool {
public:
    ThreadPool(size_t num_threads);
    ~ThreadPool();

    void submit(std::function<void()> task);

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mtx_;
    std::condition_variable cv_;
    bool stop_ = false;

    void worker_loop();
};

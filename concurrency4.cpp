#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <future>

class ThreadPool {
public:
    ThreadPool(size_t n_threads) : threads(n_threads) {
        for (size_t i = 0; i < n_threads; ++i) {
            threads[i] = std::thread(&ThreadPool::worker, this);
        }
    }

    template <typename Func>
    std::future<typename std::result_of<Func()>::type> enqueue(Func&& f) {
        std::lock_guard<std::mutex> lock(mutex);
        task_queue.emplace([&f]() { return f(); });
        cv.notify_one();
        return std::async(std::launch::deferred, [this, task = std::move(f)]() {
            (*task)();
            });
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            stop_flag = true;
        }
        cv.notify_all();
        for (auto& thread : threads) {
            thread.join();
        }
    }

private:
    void worker() {
        while (true) {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [this]() { return stop_flag || !task_queue.empty(); });
            if (stop_flag) {
                return;
            }
            auto task = std::move(task_queue.front());
            task_queue.pop();
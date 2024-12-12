#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool done = false;
int shared_counter = 0;

void increment_counter(int thread_id) {
    for (int i = 0; i < num_iterations; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            shared_counter++;
            if (shared_counter == num_iterations) {
                done = true;
                cv.notify_all();
            }
        }
    }
}

void waiter() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []() { return done; });
    }
    std::cout << "Final count after waiter: " << shared_counter << std::endl;
}

int main() {
    std::thread thread1(increment_counter, 1);
    std::thread thread2(increment_counter, 2);
    std::thread worker(waiter);

    thread1.join();
    thread2.join();
    worker.join();

    return 0;
}

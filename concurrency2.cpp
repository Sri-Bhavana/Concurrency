#include <iostream>
#include <thread>
#include <mutex>

int num_iterations = 10000000;
std::mutex mtx;
int shared_counter = 0;

void increment_counter(int thread_id) {
    for (int i = 0; i < num_iterations; ++i) {
        // Acquire the lock before incrementing
        std::lock_guard<std::mutex> lock(mtx);
        shared_counter++;
    }
}

int main() {
    std::thread thread1(increment_counter, 1);
    std::thread thread2(increment_counter, 2);

    thread1.join();
    thread2.join();

    std::cout << "Final count with mutex: " << shared_counter << std::endl;

    return 0;
}
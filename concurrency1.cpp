#include <iostream>
#include <thread>
#include <atomic>

int num_iterations = 10000000;

std::atomic<int> shared_counter{ 0 };

void increment_counter(int thread_id) {
    for (int i = 0; i < num_iterations; ++i) {
        // Increment the shared counter by one
        shared_counter++;
    }
}

int main() {
    std::thread thread1(increment_counter, 1);
    std::thread thread2(increment_counter, 2);

    thread1.join();
    thread2.join();

    std::cout << "Final count without synchronization: " << shared_counter.load() << std::endl;

    // Reset the counter for the synchronized run
    shared_counter = 0;
    std::thread thread1_sync(increment_counter, 1);
    std::thread thread2_sync(increment_counter, 2);

    thread1_sync.join();
    thread2_sync.join();
    // Correct count using std::atomic
    std::cout << "Final count with synchronization (std::atomic): " << shared_counter.load() << std::endl;

    return 0;
}
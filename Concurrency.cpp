#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>

int sharedCounter = 0;
std::mutex mtx;

void incrementCounter(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        sharedCounter++;
    }
}

int main() {
    int numThreads = 2;
    int iterations = 1000000;
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread(incrementCounter, iterations));
    }

    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    std::cout << "Final value of sharedCounter with synchronization (using std::mutex): " << sharedCounter << std::endl;

    return 0;
}
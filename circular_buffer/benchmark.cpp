#include "mutex_buffer.hpp"

#include <cassert>
#include <nanobench.h>
#include <thread>
#include <iostream>
#include <sched.h>
#include <unistd.h>
#include <pthread.h>

// Function to set thread affinity to a specific core
void set_affinity(int core_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    pthread_t thread = pthread_self();
    int result = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    if (result != 0) [[unlikely]]
    {
        std::cerr << "Error setting thread affinity to core " << core_id << std::endl;
    }
}

// Function to get the current CPU core where the thread is running
int get_current_cpu() {
    return sched_getcpu(); // Returns the core ID
}

void run(auto& buffer)
{
    std::array<std::jthread, 3> threads;

    threads[0] = std::jthread([&] {
        set_affinity(1);
        for (int i = 0; i < 1'000'000; ++i) {
            while (!buffer.push(i)) {
                continue;
            }
        }
    });

    threads[1] = std::jthread([&] {
        set_affinity(2);
        auto consume = [i = 0](int item) mutable {
            assert(item == i++);
        };
        for (int i = 0; i < 1'000'000; ++i) {
            while (!buffer.consume_one(consume)) {
                continue;
            }
        }
    });
}

int main() {
    pid_t pid = getpid();
    std::cout << "Process ID: " << pid << std::endl;

    MutexBuffer<int> mutex_buffer(10'000);

    ankerl::nanobench::Bench().run("mutex_buffer", [&] {
        run(mutex_buffer);
    });
}

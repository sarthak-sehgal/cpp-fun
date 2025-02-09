#include "mutex_buffer_v1.hpp"
#include "mutex_buffer_v2.hpp"
#include "mutex_buffer_v3.hpp"

#include <cassert>
#include <nanobench.h>
#include <thread>
#include <iostream>
#ifdef __unix__
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
#endif

void run(auto& buffer)
{
    std::array<std::jthread, 2> threads;

    threads[0] = std::jthread([&] {
        #ifdef __unix__
        set_affinity(1);
        #endif
        for (int i = 0; i < 1'000'000; ++i) {
            while (!buffer.push(i)) {
                continue;
            }
        }
    });

    threads[1] = std::jthread([&] {
        #ifdef __unix__
        set_affinity(2);
        #endif
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
    MutexBuffer_V1<int> mutex_buffer_v1(10'000);
    MutexBuffer_V2<int> mutex_buffer_v2(10'000);
    // MutexBuffer_V3<int> mutex_buffer_v3(10'000);

    ankerl::nanobench::Bench().run("mutex_buffer_v1", [&mutex_buffer_v1] {
        run(mutex_buffer_v1);
    });

    ankerl::nanobench::Bench().run("mutex_buffer_v2", [&mutex_buffer_v2] {
        run(mutex_buffer_v2);
    });

    // ankerl::nanobench::Bench().run("mutex_buffer_v3", [&mutex_buffer_v3] {
    //     run(mutex_buffer_v3);
    // });
}

#include "atomic_buffer_v1.hpp"
#include "atomic_buffer_v2.hpp"

#include <boost/lockfree/spsc_queue.hpp>

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
        set_affinity(26);
        #endif
        for (int i = 0; i < 1'000'000; ++i) {
            while (!buffer.push(i)) {
                continue;
            }
        }
    });

    threads[1] = std::jthread([&] {
        #ifdef __unix__
        set_affinity(27);
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
    AtomicBuffer_V1<int> atomic_buffer_v1(1000);
    AtomicBuffer_V2<int> atomic_buffer_v2(1000);
    boost::lockfree::spsc_queue<int> boost_buffer(1000);

    ankerl::nanobench::Bench().minEpochIterations(100).epochs(5).warmup(5).run("atomic_spsc_v1", [&atomic_buffer_v1] {
        run(atomic_buffer_v1);
    });

    ankerl::nanobench::Bench().minEpochIterations(100).epochs(5).warmup(5).run("atomic_spsc_v2", [&atomic_buffer_v2] {
        run(atomic_buffer_v2);
    });

    ankerl::nanobench::Bench().minEpochIterations(100).epochs(5).warmup(5).run("boost_spsc", [&boost_buffer] {
        run(boost_buffer);
    });
}

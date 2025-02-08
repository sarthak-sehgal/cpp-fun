#include "mutex_buffer.hpp"

#include <cassert>
#include <nanobench.h>
#include <thread>
#include <iostream>

// specific to MacOS
// #include <sys/sysctl.h> // for sysctlbyname
// #include <mach/mach.h> // for thread_policy_set()
// #include <mach/error.h> // for mach_error_string()
// #include <unistd.h>  // for getpid()

// int get_num_cores() {
//     int num_cores;
//     size_t size = sizeof(num_cores);
//     if (sysctlbyname("hw.physicalcpu", &num_cores, &size, nullptr, 0) != 0) {
//         std::cerr << "Failed to get number of physical cores" << std::endl;
//         return 0;
//     }
//     return num_cores;
// }

// void set_affinity_tag(int core_id) {
//     int num_cores = get_num_cores();
    
//     if (core_id >= num_cores) {
//         std::cerr << "Error: Invalid core_id. Only " << num_cores << " cores available." << std::endl;
//         return;
//     }

//     thread_port_t thread = pthread_mach_thread_np(pthread_self());
//     thread_affinity_policy_data_t policy;
//     policy.affinity_tag = core_id;
    
//     int result = thread_policy_set(thread, THREAD_AFFINITY_POLICY,
//                       (thread_policy_t)&policy, 1);
//     if (result != KERN_SUCCESS) {
//         std::cerr << "Error setting thread affinity! Error code: " << result << ", error: " << mach_error_string(result) << std::endl;
//     } else {
//         std::cout << "Thread pinned to core " << core_id << std::endl;
//     }
// }

// int get_current_cpu() {
//     integer_t cpu;
//     mach_msg_type_number_t count;
//     thread_basic_info_data_t info;
    
//     thread_t thread = mach_thread_self();
//     count = THREAD_BASIC_INFO_COUNT;

//     if (thread_info(thread, THREAD_BASIC_INFO, (thread_info_t)&info, &count) == KERN_SUCCESS) {
//         cpu = info.cpu_usage;  // This does not return the core ID directly, so we need another method
//     } else {
//         std::cerr << "Error getting thread info" << std::endl;
//         return -1;
//     }
    
//     return cpu;
//     // return sched_getcpu(); // This is unavailable on macOS, but you can use sysctl or affinity APIs
// }

// void run(auto& buffer)
// {
//     std::array<std::jthread, 3> threads;

//     threads[0] = std::jthread([&] {
//         // set_affinity_tag(1);
//         std::cout << "Producer running on core: " << get_current_cpu() << std::endl;
//         for (int i = 0; i < 1'000'000; ++i) {
//             while (!buffer.push(i)) {
//                 continue;
//             }
//             std::this_thread::sleep_for(std::chrono::microseconds(1));
//         }
//     });

//     threads[1] = std::jthread([&] {
//         // set_affinity_tag(2);
//         std::cout << "Producer running on core: " << get_current_cpu() << std::endl;
//         auto consume = [i = 0](int item) mutable {
//             assert(item == i++);
//         };
//         for (int i = 0; i < 1'000'000; ++i) {
//             while (!buffer.consume_one(consume)) {
//                 continue;
//             }
//         }
//     });
// }

int main() {
    pid_t pid = getpid();
    std::cout << "Process ID: " << pid << std::endl;

    MutexBuffer<int> mutex_buffer(10'000);

    ankerl::nanobench::Bench().run("mutex_buffer", [&] {
        run(mutex_buffer);
    });
}
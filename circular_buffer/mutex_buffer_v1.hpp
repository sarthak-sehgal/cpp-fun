#include <mutex>
#include <vector>


template <typename T>
class MutexBuffer_V1 {
    #if defined(__cpp_lib_hardware_interference_size)
    static constexpr std::size_t CL_SIZE = std::hardware_constructive_interference_size;
    #else
    // fallback value
    static constexpr std::size_t CL_SIZE = 64;
    #endif
public:
    MutexBuffer_V1(size_t capacity) : buffer(capacity)
    {
        buffer.resize(capacity);
    }

    bool push(const T& item)
    {
        std::lock_guard<std::mutex> lock(mutex);
        std::size_t next_tail = (tail + 1) % buffer.size();
        if (next_tail == head)
            return false;
        buffer[tail] = item;
        tail = next_tail;
        return true;
    }

    bool consume_one(auto&& func)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (head == tail)
            return false;
        func(std::move(buffer[head]));
        head = (head + 1) % buffer.size();
        return true;
    }

private:
    std::vector<T> buffer;
    std::mutex mutex;
    std::size_t head = 0;
    std::size_t tail = 0;
};

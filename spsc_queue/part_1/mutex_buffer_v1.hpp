#pragma once

#include <mutex>
#include <vector>

template <typename T>
class MutexBuffer_V1 {
public:
    MutexBuffer_V1(size_t capacity)
    : capacity{capacity}
    {
        buffer.resize(capacity);
    }

    bool push(const T& item)
    {
        std::lock_guard<std::mutex> lock(mutex);
        std::size_t next_tail = (tail + 1) % capacity;
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
        T elem = std::move(buffer[head]);
        func(std::move(elem));
        head = (head + 1) % capacity;
        return true;
    }

private:
    std::size_t const capacity;
    std::size_t head = 0;
    std::size_t tail = 0;
    std::vector<T> buffer;
    std::mutex mutex;
};

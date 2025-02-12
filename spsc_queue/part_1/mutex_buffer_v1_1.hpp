#pragma once

#include <cassert>
#include <cstddef>
#include <mutex>
#include <stdlib.h>
#include <vector>

template <typename T>
class MutexBuffer_V1_1 {
public:
    MutexBuffer_V1_1(size_t capacity)
    : capacity(capacity)
    {
        buffer = static_cast<Element*>(malloc(capacity*sizeof(Element)));
        assert(buffer != nullptr);
    }

    bool push(const T& item)
    {
        std::lock_guard<std::mutex> lock(mutex);
        std::size_t const nextTail = increment(tail);
        if (nextTail == head)
            return false;

        std::construct_at(reinterpret_cast<T*>(buffer + tail), item);
        tail = nextTail;
        return true;
    }

    bool consume_one(auto&& func)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (head == tail)
            return false;
        T& elem = *reinterpret_cast<T*>(buffer + tail);
        func(elem);
        std::destroy_at(&elem);

        head = increment(head);
        return true;
    }

    ~MutexBuffer_V1_1()
    {
        auto noop = [](T&) {};
        while(consume_one(noop));
        free(buffer);
    }

private:
    struct Element
    {
        alignas(T) std::byte storage[sizeof(T)];
    };

    std::size_t increment(std::size_t index)
    {
        return (index + 1) % capacity;
    }

    std::size_t const capacity;
    std::size_t head = 0;
    std::size_t tail = 0;
    Element* buffer;
    std::mutex mutex;
};

#pragma once

#include <atomic>
#include <cassert>
#include <cstddef>
#include <stdlib.h>
#include <vector>

template <typename T>
class AtomicBuffer_V1 {
public:
    AtomicBuffer_V1(size_t capacity_)
    : capacity{capacity_+1}
    {
        buffer = static_cast<Element*>(malloc(capacity*sizeof(Element)));
        assert(buffer != nullptr);
    }

    bool push(const T& item)
    {
        std::size_t const currTail = tail.load();
        std::size_t const nextTail = increment(currTail);
        if (nextTail == head.load())
            return false;

        std::construct_at(reinterpret_cast<T*>(&buffer[currTail]), item);
        tail = nextTail;
        return true;
    }

    bool consume_one(auto&& func)
    {
        std::size_t const currHead = head.load();
        if (currHead == tail)
            return false;

        T& elem = *reinterpret_cast<T*>(&buffer[currHead]);
        func(elem);
        std::destroy_at(&elem);

        head = increment(currHead);
        return true;
    }

    ~AtomicBuffer_V1()
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
    std::atomic<std::size_t> head = 0;
    std::atomic<std::size_t> tail = 0;
    Element* buffer;
};

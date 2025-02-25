#pragma once

#include <atomic>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <new>
#include <stdlib.h>
#include <vector>

template <typename T>
class AtomicBuffer_V4 {
public:
    #ifdef __cpp_lib_hardware_interference_size
        static constexpr std::size_t hardware_destructive_interference_size = std::hardware_destructive_interference_size;
    #else
        static constexpr std::size_t hardware_destructive_interference_size = 64;
    #endif

    AtomicBuffer_V4(size_t capacity_)
    : capacity{capacity_+1}
    {
        buffer = static_cast<Element*>(malloc(capacity*sizeof(Element)));
        assert(buffer != nullptr);
    }

    bool push(const T& item)
    {
        std::size_t const currTail = tail.load(std::memory_order_relaxed);
        std::size_t const nextTail = currTail+1 == capacity ? 0 : currTail+1;
        if (nextTail == headCache)
        {
            headCache = head.load(std::memory_order_acquire);
            if (nextTail == headCache)
                return false;
        }

        new (buffer + currTail) T(item);
        tail.store(nextTail, std::memory_order_release);
        return true;
    }

    bool consume_one(auto&& func)
    {
        std::size_t const currHead = head.load(std::memory_order_relaxed);
        if (currHead == tailCache)
        {
            tailCache = tail.load(std::memory_order_acquire);
            if (currHead == tailCache)
                return false;
        }

        T& elem = *reinterpret_cast<T*>(buffer + currHead);
        func(elem);
        elem.~T();

        std::size_t nextHead = currHead + 1 == capacity ? 0 : currHead+1;
        head.store(nextHead, std::memory_order_release);
        return true;
    }

    ~AtomicBuffer_V4()
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

    std::size_t const capacity;
    Element* buffer;

    alignas(hardware_destructive_interference_size) std::atomic<std::size_t> head = 0;
    std::size_t tailCache = 0;

    alignas(hardware_destructive_interference_size) std::atomic<std::size_t> tail = 0;
    std::size_t headCache = 0;
};

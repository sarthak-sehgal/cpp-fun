#include <cassert>
#include <cstddef>
#include <mutex>
#include <vector>

template <typename T>
class MutexBuffer_V3 {
public:
    MutexBuffer_V3(size_t capacity)
    {
        buffer = reinterpret_cast<std::byte*>(malloc(capacity*sizeof(T)));
        assert(buffer != nullptr);
        endPtr = buffer + capacity*sizeof(T);
        head = buffer;
        tail = buffer;
    }

    bool push(const T& item)
    {
        std::lock_guard<std::mutex> lock(mutex);
        auto* nextTail = increment(tail);
        if (nextTail == head)
            return false;
        
        std::construct_at(reinterpret_cast<T*>(tail), item);
        tail = nextTail;
        return true;
    }

    bool consume_one(auto&& func)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (head == tail)
            return false;
        T elem = std::move(*reinterpret_cast<T*>(head));
        func(std::move(elem));
        head = increment(head);
        return true;
    }

    ~MutexBuffer_V2()
    {
        free(buffer);
    }

private:
    std::byte* increment(std::byte* ptr)
    {
        auto* nextPtr = ptr + sizeof(T);
        return nextPtr == endPtr ? buffer : nextPtr;
    }

    alignas(T) std::byte* buffer;
    std::byte* endPtr;
    std::mutex mutex;
    std::byte* head;
    std::byte* tail;
};

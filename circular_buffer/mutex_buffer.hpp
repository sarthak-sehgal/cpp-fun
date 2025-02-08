#include <mutex>
#include <vector>

template <typename T>
class MutexBuffer {
public:
    MutexBuffer(size_t capacity) : buffer(capacity)
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

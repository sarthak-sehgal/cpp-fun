#pragma once

#include <concepts>
#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <type_traits>

struct ThreadPool
{
    ThreadPool(std::size_t numThreads)
    {
        setupThreads(numThreads);
    }

    template <std::invocable Func>
    auto enqueueTask(Func&& task)
    {
        using ReturnType = std::invoke_result_t<Func>;
        std::promise<ReturnType> promise;
        auto future = promise.get_future();
        auto qTask = [promise = std::move(promise), task = std::forward<Func>(task)]() mutable {
            if constexpr (std::is_same_v<ReturnType, void>)
            {
                task();
                promise.set_value();
            }
            else
            {
                promise.set_value(task());
            }
        };

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.push(std::move(qTask));
        }
        queueCv.notify_all();
        return future;
    }

    void finish()
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
        lock.unlock();
        queueCv.notify_all();
    }

    ~ThreadPool()
    {
        finish();
    }

private:
    void setupThreads(std::size_t numThreads)
    {
        for (auto i = 0u; i < numThreads; ++i)
        {
            threads.emplace_back([this] {
                while (true)
                {
                    std::move_only_function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        queueCv.wait(lock, [this] { return !tasks.empty() || stop; });
                        if (stop && tasks.empty())
                            return;
                        if (tasks.empty()) // spurious wake up
                            continue;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    std::queue<std::move_only_function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable queueCv;
    std::vector<std::jthread> threads;
    bool stop = false;
};
#include "thread_pool.hpp"

#include <iostream>

int main()
{
    ThreadPool pool(2);

    auto future1 = pool.enqueueTask([] {
        int sum = 0;
        for (int i = 1; i <= 250; ++i)
            sum += i;
        return sum;
    });

    auto future2 = pool.enqueueTask([] {
        int sum = 0;
        for (int i = 251; i <= 500; ++i)
            sum += i;
        return sum;
    });

    auto future3 = pool.enqueueTask([] {
        int sum = 0;
        for (int i = 501; i <= 750; ++i)
            sum += i;
        return sum;
    });

    auto future4 = pool.enqueueTask([] {
        int sum = 0;
        for (int i = 751; i <= 1000; ++i)
            sum += i;
        return sum;
    });

    std::cout << "Sum of 1 to 1000: " << future1.get() + future2.get() + future3.get() + future4.get() << '\n';
}
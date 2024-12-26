#include "profiler.hpp"

#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>

static constexpr std::size_t BUFFER_SIZE = 1024*1024*16; // 16MB

__attribute__((noinline)) void read(char* buffer1, char* buffer2, std::size_t stepSize)
{
    for (std::size_t i = 0; i < BUFFER_SIZE; i += stepSize)
    {
        buffer2[i] = buffer1[i];
    }
}

__attribute__((noinline)) Stats runTest(std::size_t stepSize)
{
    std::time_t now = std::time(nullptr);
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, now & 0b111111);

    Profiler profiler{stepSize};
    static constexpr auto numTimes = 10u;

    for (auto i = 0u; i < numTimes; ++i)
    {
        char *buffer1 = new char[BUFFER_SIZE];
        char *buffer2 = new char[BUFFER_SIZE];
        for (std::size_t i = 0; i < BUFFER_SIZE; ++i)
            buffer1[i] = dist(rng);

        profiler.reset();
        for (std::size_t j = 0; j < stepSize; ++j)
        {
            read(buffer1, buffer2, stepSize);
        }
        profiler.lap();

        delete[] buffer1;
        delete[] buffer2;
    }

    return profiler.get();
}

int main (int argc, char* argv[])
{
    assert(argc == 2);
    std::ofstream statFile;
    statFile.open(argv[1], std::ios_base::app);

    for (std::size_t stepSize = 4u; stepSize <= 512u; ++stepSize)
    {
        auto [avg, max, min] = runTest(stepSize);
        statFile << std::fixed << std::showpoint << std::setprecision(2);
        statFile << stepSize << ',' << (double)BUFFER_SIZE/avg << ',' << (double)BUFFER_SIZE/max
            << ',' <<  (double)BUFFER_SIZE/min << '\n';

        std::cout << std::fixed << std::showpoint << std::setprecision(2);
        std::cout << stepSize << '\t' << (double)BUFFER_SIZE/avg << '\t' << (double)BUFFER_SIZE/max
            << '\t' <<  (double)BUFFER_SIZE/min << '\n';
    }
}
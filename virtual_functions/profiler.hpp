#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <limits>
#include <time.h>

struct Stats
{
    double avg;
    std::size_t max;
    std::size_t min;
};

struct Profiler {
    Profiler();
    ~Profiler();
    void reset();
    void lap();
    Stats get() const;

private:
    static std::uint64_t getTimeDiff(timespec const& start, timespec const& end);

    std::ofstream file;
    std::size_t numTimes = 0;
    timespec startTime;
    std::uint64_t total = 0;
    std::uint64_t min = std::numeric_limits<std::uint64_t>::max();
    std::uint64_t max = std::numeric_limits<std::uint64_t>::min();
};
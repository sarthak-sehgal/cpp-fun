#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <time.h>

struct Stats
{
    double avg;
    double max;
    double min;
};

struct Profiler {
    Profiler(std::size_t stepSize);
    void reset();
    void lap();
    Stats get() const;

private:
    static std::uint64_t getTimeDiff(timespec const& start, timespec const& end);

    std::size_t const stepSize;
    std::size_t numTimes = 0;
    timespec startTime;
    std::uint64_t total = 0;
    std::uint64_t min = std::numeric_limits<std::uint64_t>::max();
    std::uint64_t max = std::numeric_limits<std::uint64_t>::min();
};
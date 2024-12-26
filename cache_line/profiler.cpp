#include "profiler.hpp"

#include <algorithm>

Profiler::Profiler(std::size_t stepSize)
    : stepSize{stepSize}
{}

void Profiler::reset()
{
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
}

void Profiler::lap()
{
    timespec endTime;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
    
    auto const currTimeNs = getTimeDiff(startTime, endTime);
    ++numTimes;
    total += currTimeNs;
    max = std::max(max, currTimeNs);
    min = std::min(min, currTimeNs);
}

std::uint64_t Profiler::getTimeDiff(timespec const& start, timespec const& end)
{
    timespec diff = {end.tv_sec - start.tv_sec, end.tv_nsec - start.tv_nsec};
    if (diff.tv_nsec < 0)
    {
        diff.tv_sec -= 1;
        diff.tv_nsec += 1'000'000'000;
    }
    return diff.tv_sec*1'000'000'000 + diff.tv_nsec;
}

Stats Profiler::get() const
{
    return {(double)total/(numTimes*stepSize), (double)max/stepSize, (double)min/stepSize};
}
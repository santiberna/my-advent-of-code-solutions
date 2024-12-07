#pragma once
#include <chrono>
#include <Utility/Log.hpp>

using DeltaMS = std::chrono::duration<float, std::milli>;

class Stopwatch
{
public:
    Stopwatch();
    DeltaMS GetElapsed() const;
    void Reset();

private:
    std::chrono::high_resolution_clock::time_point start;
};

class ScopedTimer
{
public:
    ScopedTimer(const char* name)
        : name(name)
    {
    }
    ~ScopedTimer() { Log::Debug("{}: {}ms", name, watch.GetElapsed().count()); }

private:
    const char* name {};
    Stopwatch watch {};
};

#define PROFILE_SCOPE() auto _timer_ = ScopedTimer(__FUNCTION__)
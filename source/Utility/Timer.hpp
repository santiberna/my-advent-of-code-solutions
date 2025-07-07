#pragma once
#include <chrono>
#include <iostream>

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
    ScopedTimer(const std::string& name)
        : name(name)
    {
    }
    
    ~ScopedTimer() { 
        std::cout << "--- PROFILE RESULT (" << name << "): " << watch.GetElapsed().count() << "ms\n";
    }

private:
    std::string name {};
    Stopwatch watch {};
};

#define PROFILE_SCOPE() auto _timer_ = ScopedTimer(__FUNCTION__)
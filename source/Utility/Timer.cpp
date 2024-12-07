#include <Utility/Timer.hpp>

Stopwatch::Stopwatch()
{
    Reset();
}

DeltaMS Stopwatch::GetElapsed() const
{
    return std::chrono::duration_cast<DeltaMS>(std::chrono::high_resolution_clock::now() - start);
}

void Stopwatch::Reset()
{
    start = std::chrono::high_resolution_clock::now();
}
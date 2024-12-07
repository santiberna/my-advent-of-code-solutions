#pragma once
#include <glm/glm.hpp>

inline glm::ivec2 TurnVec2(const glm::vec2& rot, bool clockwise)
{
    if (!clockwise)
    {
        return { -rot.y, rot.x };
    }
    return { rot.y, -rot.x };
}

inline int64_t PowInt(int64_t x, int64_t y)
{
    int64_t out = 1;
    while (y--)
    {
        out *= x;
    }
    return out;
}

inline int64_t NumDigits(int64_t x)
{
    int64_t out = 0;

    while (x)
    {
        out++;
        x /= 10;
    }

    return out;
}

// Hash functions for math types:

constexpr size_t BIG_PRIME_ARRAY[] = {
    50331653,
    25165843
};

namespace std
{

template <>
struct hash<glm::ivec2>
{
    size_t operator()(const glm::ivec2& k) const
    {
        return static_cast<size_t>(k.x) * BIG_PRIME_ARRAY[0] ^ static_cast<size_t>(k.y) * BIG_PRIME_ARRAY[1];
    }
};

}
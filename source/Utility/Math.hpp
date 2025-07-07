#pragma once

#include <cmath>
#include <cstdlib>
#include <utility>
#include <ostream>

namespace math
{

template<typename T>
inline int sign(const T& val)
{
    return (T(0) < val) - (val < T(0));
}

template<typename IntType>
IntType powInteger(IntType base, IntType exp)
{
    IntType out = 1;
    while (exp--)
    {
        out *= base;
    }
    return out;
}

template<typename IntType>
IntType numDigits(IntType num)
{
    IntType out = 0;

    while (num)
    {
        out++;
        num /= 10;
    }

    return out;
}

inline bool floatEq(float a, float b, float eps = 1e-5f)
{
    return std::abs(a - b) < eps;
}

inline bool doubleEq(double a, double b, double eps = 1e-9f)
{
    return std::abs(a - b) < eps;
}

template<typename T>
struct Vec2 {

    T x{}, y{};

    Vec2() = default;
    Vec2(T x_, T y_) : x(x_), y(y_) {}

    template<typename U>
    Vec2(const Vec2<U>& o) : x(o.x), y(o.y) {}

    Vec2 operator-() const { return Vec2{-x, -y}; }

    Vec2 operator+(const Vec2& rhs) const { return Vec2{x + rhs.x, y + rhs.y}; }
    Vec2 operator-(const Vec2& rhs) const { return Vec2{x - rhs.x, y - rhs.y}; }
    Vec2 operator*(const Vec2& rhs) const { return Vec2{x * rhs.x, y * rhs.y}; }
    Vec2 operator/(const Vec2& rhs) const { return Vec2{x / rhs.x, y / rhs.y}; }

    Vec2 operator*(const T& r) const { return {x * r, y * r }; }
    Vec2 operator/(const T& r) const { return {x / r, y / r }; }

    Vec2& operator+=(const Vec2& rhs) { x += rhs.x; y += rhs.y; return *this; }
    Vec2& operator-=(const Vec2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    Vec2& operator*=(const Vec2& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
    Vec2& operator/=(const Vec2& rhs) { x /= rhs.x; y /= rhs.y; return *this; }

    bool operator==(const Vec2& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Vec2& rhs) const { return !(*this == rhs); }
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v) { return os << '(' << v.x << ", " << v.y << ')'; }
};

template<typename T>
struct Vec3
{
    T x{}, y{}, z{};

    Vec3() = default;
    Vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

    template<typename U>
    Vec3(const Vec3<U>& o) : x(o.x), y(o.y), z(o.z) {}

    Vec3 operator-() const { return {-x, -y, -z}; }
    Vec3 operator+(const Vec3& r) const { return {x + r.x, y + r.y, z + r.z}; }
    Vec3 operator-(const Vec3& r) const { return {x - r.x, y - r.y, z - r.z}; }
    Vec3 operator*(const Vec3& r) const { return {x * r.x, y * r.y, z * r.z}; }
    Vec3 operator/(const Vec3& r) const { return {x / r.x, y / r.y, z / r.z}; }

    Vec3 operator*(const T& r) const { return {x * r, y * r, z * r}; }
    Vec3 operator/(const T& r) const { return {x / r, y / r, z / r}; }

    Vec3& operator+=(const Vec3& r) { x += r.x; y += r.y; z += r.z; return *this; }
    Vec3& operator-=(const Vec3& r) { x -= r.x; y -= r.y; z -= r.z; return *this; }
    Vec3& operator*=(const Vec3& r) { x *= r.x; y *= r.y; z *= r.z; return *this; }
    Vec3& operator/=(const Vec3& r) { x /= r.x; y /= r.y; z /= r.z; return *this; }

    bool operator==(const Vec3& r) const { return x == r.x && y == r.y && z == r.z; }
    bool operator!=(const Vec3& r) const { return !(*this == r); }

    friend std::ostream& operator<<(std::ostream& os, const Vec3& v) { return os << '(' << v.x << ", " << v.y << ", " << v.z << ')'; }
};

// Rotates a vector by 90 degrees
template<typename T>
Vec2<T> rotate(const Vec2<T>& v, bool clockwise)
{
    if (!clockwise)
    {
        return { -v.y, v.x };
    }
    return { v.y, -v.x };
}

template<typename FloatType>
double length(const Vec2<FloatType>& v)
{
    return sqrt(static_cast<double>(v.x * v.x + v.y * v.y));
}

template<typename FloatType>
double length(const Vec3<FloatType>& v)
{
    return sqrt(static_cast<double>(v.x * v.x + v.y * v.y + v.z * v.z));
}

template<typename FloatType>
Vec2<FloatType> normalize(const Vec2<FloatType>& v)
{
    auto l = length(v);
    return v / static_cast<FloatType>(l);
}

template<typename T>
T dot_product(const Vec2<T>& v, const Vec2<T>& u)
{
    return v.x * u.x + v.y * u.y;
}

template<typename T>
T dot_product(const Vec3<T>& v, const Vec3<T>& u)
{
    return v.x * u.x + v.y * u.y + v.z * v.y;
}

template<typename T>
T cross_product(const Vec2<T>& v, const Vec2<T>& u)
{
    return v.x * u.y - v.y * u.x; 
}

template<typename T>
T manhattan_distance(const Vec2<T>& v)
{
    return std::abs(v.x) + std::abs(v.y);
}

template<typename T>
T manhattan_distance(const Vec3<T>& v)
{
    return std::abs(v.x) + std::abs(v.y) + std::abs(v.z);
}

};

// inline int intSign(int val)
// {
//     return (0 < val) - (val < 0);
// }

// struct IVec2
// {
//     int x {}, y {};

//     IVec2 operator+(const IVec2& other) const
//     {
//         return { x + other.x, y + other.y };
//     }

//     IVec2 operator-(const IVec2& other) const
//     {
//         return { x - other.x, y - other.y };
//     }
// };

// inline int64_t PowInt(int64_t x, int64_t y)
// {
//     int64_t out = 1;
//     while (y--)
//     {
//         out *= x;
//     }
//     return out;
// }

// inline int64_t NumDigits(int64_t x)
// {
//     int64_t out = 0;

//     while (x)
//     {
//         out++;
//         x /= 10;
//     }

//     return out;
// }

// inline float length(const std::pair<float, float>& v)
// {
//     return std::sqrtf(v.first * v.first + v.second * v.second);
// }

// inline std::pair<float, float> normalize(const std::pair<float, float>& v)
// {
//     auto l = length(v);
//     return { v.first / l, v.second / l};
// }

// inline float dot_product(const std::pair<float, float>& v, const std::pair<float, float>& u)
// {
//     return v.first * u.first + v.second * u.second;
// }

// inline float cross_product(const std::pair<float, float>& v, const std::pair<float, float>& u)
// {
//     return v.first * u.second - v.second * u.first; 
// }

// inline bool float_eq(float a, float b, float eps = 1e-5f)
// {
//     return std::abs(a - b) < eps;
// }

// // double

// inline double length(const std::pair<double, double>& v)
// {
//     return std::sqrt(v.first * v.first + v.second * v.second);
// }

// inline std::pair<double, double> normalize(const std::pair<double, double>& v)
// {
//     auto l = length(v);
//     return { v.first / l, v.second / l};
// }

// inline double dot_product(const std::pair<double, double>& v, const std::pair<double, double>& u)
// {
//     return v.first * u.first + v.second * u.second;
// }

// inline double cross_product(const std::pair<double, double>& v, const std::pair<double, double>& u)
// {
//     return v.first * u.second - v.second * u.first; 
// }

// inline bool double_eq(double a, double b, double eps = 1e-9f)
// {
//     return std::abs(a - b) < eps;
// }

// Hash functions for math types:

namespace std
{

template<typename T>
struct hash<math::Vec2<T>> 
{
    size_t operator()(const math::Vec2<T>& v) const 
    {
        size_t h1 = std::hash<T>{}(v.x);
        size_t h2 = std::hash<T>{}(v.y);

        size_t seed = h1;
        seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

template<typename T>
struct hash<math::Vec3<T>> 
{
    size_t operator()(const math::Vec3<T>& v) const 
    {
        size_t h1 = std::hash<T>{}(v.x);
        size_t h2 = std::hash<T>{}(v.y);
        size_t h3 = std::hash<T>{}(v.z);

        size_t seed = h1;
        seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

}

// template <typename T, typename U>
// std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& p)
// {
//     os << "(" << p.first << ", " << p.second << ")";
//     return os;
// }
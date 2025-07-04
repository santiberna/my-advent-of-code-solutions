#pragma once
#include <array>
#include <vector>
#include <optional>
#include <iostream>

template <typename T>
class Array2D
{
public:
    Array2D() = default;
    Array2D(size_t width, size_t height);
    Array2D(size_t width, size_t height, const T& init);
    size_t GetWidth() const { return width; }
    size_t GetHeight() const { return height; }

    // Top left corner
    template <size_t W, size_t H>
    std::optional<std::array<T, W * H>> CopySlice(size_t ci, size_t cj) const;

    const T* operator()(size_t i, size_t j) const;
    T* operator()(size_t i, size_t j);

    T GetOr(size_t i, size_t j, const T& val) const;

    void Print() const;

private:
    std::vector<T> data;
    size_t width {};
    size_t height {};
};

template <typename T>
inline void Array2D<T>::Print() const
{
    for (size_t j = 0; j < GetHeight(); j++)
    {
        for (size_t i = 0; i < GetWidth(); i++)
        {
            std::cout << *this->operator()(i, j);
        }
        std::cout << '\n';
    }
}

template <typename T>
inline Array2D<T>::Array2D(size_t width, size_t height)
    : data(width * height)
    , width(width)
    , height(height)
{
}

template <typename T>
inline Array2D<T>::Array2D(size_t width, size_t height, const T& init)
    : data(width * height, init)
    , width(width)
    , height(height)
{
}

template <typename T>
inline const T* Array2D<T>::operator()(size_t i, size_t j) const
{
    if (i >= width || j >= height)
        return nullptr;
    return &data[j * width + i];
}

template <typename T>
inline T* Array2D<T>::operator()(size_t i, size_t j)
{
    if (i >= width || j >= height)
        return nullptr;
    return &data[j * width + i];
}

template <typename T>
inline T Array2D<T>::GetOr(size_t i, size_t j, const T& val) const
{
    if (auto p = this->operator()(i, j))
    {
        return *p;
    }
    return val;
}

template <typename T>
template <size_t W, size_t H>
std::optional<std::array<T, W * H>> Array2D<T>::CopySlice(size_t ci, size_t cj) const
{
    if (ci + W > width || cj + H > height)
    {
        return std::nullopt;
    }

    std::array<T, W * H> out {};

    for (auto j = 0; j < H; j++)
    {
        for (auto i = 0; i < W; i++)
        {
            out.at(i + j * W) = *(*this)(i + ci, j + cj);
        }
    }

    return out;
}
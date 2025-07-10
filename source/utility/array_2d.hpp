#pragma once
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <string_view>
#include <ostream>

// Iterator for Array2D, also allows for getting the 2D position of the element in the array
template <typename T>
class Array2DIterator
{
private:
    static constexpr bool IS_CONST = std::is_const_v<T>;

    using Self = Array2DIterator<T>;
    using Value = std::remove_const_t<T>;

    using UnderlyingIterator = std::conditional_t<
        IS_CONST,
        typename std::vector<Value>::const_iterator,
        typename std::vector<Value>::iterator>;

public:
    Array2DIterator() = default;
    Array2DIterator(UnderlyingIterator base, UnderlyingIterator iterator, uint32_t stride);

    T& operator*();
    T* operator->();
    Self& operator++();
    Self& operator++(int);
    bool operator==(const Self& other) const;
    bool operator!=(const Self& other) const;

    // Returns the array position of the iterator
    std::pair<size_t, size_t> getPosition() const;

private:
    UnderlyingIterator base {};
    UnderlyingIterator iterator {};
    size_t stride {};
};

// Fixed size 2D Array
template <typename T>
class Array2D
{
public:
    Array2D() = default;
    Array2D(size_t width, size_t height);
    Array2D(size_t width, size_t height, const T& init);

    T& at(size_t i, size_t j);
    const T& at(size_t i, size_t j) const;

    size_t getWidth() const { return width; }
    size_t getHeight() const { return height; }

    void fill(const T& val);

    Array2DIterator<T> begin();
    Array2DIterator<T> end();

    Array2DIterator<const T> begin() const;
    Array2DIterator<const T> end() const;

private:
    size_t width {}, height {};
    std::vector<T> data;
};

template <typename T>
Array2DIterator<T>::Array2DIterator(UnderlyingIterator base, UnderlyingIterator iterator, uint32_t stride)
    : base(base)
    , iterator(iterator)
    , stride(stride)
{
}

template <typename T>
T& Array2DIterator<T>::operator*()
{
    return *iterator;
}

template <typename T>
T* Array2DIterator<T>::operator->()
{
    return &(*iterator);
}

template <typename T>
Array2DIterator<T>& Array2DIterator<T>::operator++()
{
    ++iterator;
    return *this;
}

template <typename T>
Array2DIterator<T>& Array2DIterator<T>::operator++(int)
{
    Self temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
bool Array2DIterator<T>::operator==(const Self& other) const
{
    return iterator == other.iterator;
}

template <typename T>
bool Array2DIterator<T>::operator!=(const Self& other) const
{
    return !(*this == other);
}

template <typename T>
std::pair<size_t, size_t> Array2DIterator<T>::getPosition() const
{
    auto offset = std::distance(base, iterator);
    return {
        static_cast<size_t>(offset % stride),
        static_cast<size_t>(offset / stride)
    };
}

template <typename T>
Array2D<T>::Array2D(size_t width, size_t height)
    : width(width)
    , height(height)
    , data(width * height)
{
}

template <typename T>
Array2D<T>::Array2D(size_t width, size_t height, const T& init)
    : width(width)
    , height(height)
    , data(width * height, init)
{
}

template <typename T>
void Array2D<T>::fill(const T& val)
{
    std::fill(data.begin(), data.end(), val);
}

template <typename T>
T& Array2D<T>::at(size_t x, size_t y)
{
    if (x >= width || y >= height)
    {
        throw std::out_of_range("Array2D index out of bounds");
    }
    return data[y * width + x];
}

template <typename T>
const T& Array2D<T>::at(size_t x, size_t y) const
{
    if (x >= width || y >= height)
    {
        throw std::out_of_range("Array2D index out of bounds");
    }
    return data[y * width + x];
}

template <typename T>
Array2DIterator<T> Array2D<T>::begin()
{
    Array2DIterator<T> it(data.begin(), data.begin(), width);
    return it;
}

template <typename T>
Array2DIterator<T> Array2D<T>::end()
{
    Array2DIterator<T> it(data.begin(), data.end(), width);
    return it;
}

template <typename T>
Array2DIterator<const T> Array2D<T>::begin() const
{
    Array2DIterator<const T> it(data.begin(), data.begin(), width);
    return it;
}

template <typename T>
Array2DIterator<const T> Array2D<T>::end() const
{
    Array2DIterator<const T> it(data.begin(), data.end(), width);
    return it;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Array2D<T>& p)
{
    for (size_t j = 0; j < p.getHeight(); ++j)
    {
        for (size_t i = 0; i < p.getWidth(); ++i)
        {
            os << p.at(i, j) << ' ';
        }
        os << "\n";
    }
    return os;
}
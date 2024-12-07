#pragma once
#include <cstdint>

template <typename It>
class SkipIndexIterator
{
    using Self = SkipIndexIterator<It>;

public:
    SkipIndexIterator(It _current, size_t _skip_index)
        : current(_current)
        , skip_index(_skip_index)
    {
        if (skip_index == 0)
        {
            ++current;
        }
        skip_index--;
    }

    auto* operator->() { return current.operator->(); }
    auto& operator*() { return current.operator*(); }

    // Increment operator (prefix)
    Self& operator++()
    {
        ++current;

        if (skip_index == 0)
        {
            ++current;
        }

        skip_index--;
        return *this;
    }

    // Increment operator (postfix)
    Self operator++(int)
    {
        auto temp = *this;
        this->operator++();
        return temp;
    }

    // Equality comparison
    bool operator==(const It& other) const
    {
        return current == other;
    }

    // Inequality comparison
    bool operator!=(const It& other) const
    {
        return current != other;
    }

private:
    It current {};
    size_t skip_index {};
};

template <typename It>
SkipIndexIterator<It> MakeSkipIndexIterator(It i, size_t skip_index)
{
    return SkipIndexIterator<It>(i, skip_index);
}
#include "combinations.hpp"

#include <algorithm>

Array2D<int> findCombinations(const std::vector<int>& set, size_t k)
{
    Array2D<int> out { k, numberOfCombinations(set.size(), k) };

    std::vector<int> v(set.size());
    std::fill(v.begin() + k, v.end(), 1);

    size_t row = 0;

    do
    {
        size_t column = 0;
        for (size_t i = 0; i < set.size(); ++i)
        {
            if (!v[i])
            {
                out.at(column++, row) = set[i];
            }
        }
        row++;

    } while (std::next_permutation(v.begin(), v.end()));

    return out;
}

Array2D<int> findPermutations(const std::vector<int>& set, size_t k)
{
    Array2D<int> out { k, numberOfPermutations(set.size(), k) };

    std::vector<int> v(set.size());

    for (int i = 0; i < v.size(); ++i)
    {
        v[i] = i;
    }
    std::fill(v.begin() + k, v.end(), 1);

    size_t row = 0;

    do
    {
        for (size_t i = 0; i < k; ++i)
        {
            out.at(i, row) = set.at(v.at(i));
        }
        row++;

    } while (std::next_permutation(v.begin(), v.end()));

    return out;
}

size_t factorial(size_t n)
{
    size_t out = n;
    while (--n)
    {
        out *= n;
    }
    return out;
}

size_t numberOfPermutations(size_t set_size, size_t comb_size)
{
    if (set_size < comb_size)
        return 0;

    size_t t = set_size - comb_size;
    size_t q = set_size;

    while (--set_size > t)
    {
        q *= set_size;
    }

    return q;
}

size_t numberOfCombinations(size_t set_size, size_t comb_size)
{
    if (set_size < comb_size)
        return 0;
    return numberOfPermutations(set_size, comb_size) / factorial(comb_size);
}

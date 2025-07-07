#pragma once
#include <utility/array_2d.hpp>

size_t factorial(size_t n);

size_t numberOfPermutations(size_t set_size, size_t comb_size);
size_t numberOfCombinations(size_t set_size, size_t comb_size);

Array2D<int> findCombinations(const std::vector<int>& set, size_t k);
Array2D<int> findPermutations(const std::vector<int>& set, size_t k);
#include "utility/timer.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <vector>

constexpr std::array<int64_t, 4> PATTERN = { 0, 1, 0, -1 };

int64_t sample_phase_pattern(int64_t step, int64_t index)
{
    return PATTERN.at(((index + 1) / step) % PATTERN.size());
}

int64_t getFirstDigits(const std::vector<int64_t>& vec, int64_t n)
{
    std::string num;
    for (size_t i = 0; i < n; i++)
    {
        num.push_back('0' + vec.at(i));
    }

    return std::atoll(num.c_str());
}

REGISTER_CHALLENGE(Y2019_CG, "input/Y2019/C16.txt")
{
    std::vector<int64_t> digits {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto line = TextHelpers::StreamToString(stream.value());

        for (auto c : line)
        {
            digits.emplace_back(c - '0');
        }
    }

    int64_t answer1 {}, answer2 {};

    { // Part 1

        std::vector<int64_t> current_digits = digits;

        for (size_t i = 0; i < 100; i++)
        {
            for (size_t i = 0; i < current_digits.size(); i++)
            {
                int64_t sum = 0;
                for (size_t j = i; j < current_digits.size(); j++)
                {
                    sum += sample_phase_pattern(i + 1, j) * current_digits[j];
                }
                current_digits.at(i) = std::abs(sum % 10);
            }
        }

        answer1 = getFirstDigits(current_digits, 8);
    }

    { // Part 2

        int64_t offset_digits = getFirstDigits(digits, 7);
        int64_t digit_count = digits.size() * 10'000 - offset_digits;

        std::vector<int64_t> current_digits {};
        current_digits.resize(digit_count);

        for (size_t i = 0; i < digit_count; i++)
        {
            auto val = digits.at((i + offset_digits) % digits.size());
            current_digits.at(i) = val;
        }

        for (size_t i = 0; i < 100; i++)
        {
            int64_t sum = current_digits.back();

            // Sum of all elements from the end, no need to sample phase
            for (int64_t i = digit_count - 2; i >= 0; i--)
            {
                sum += current_digits.at(i);
                current_digits.at(i) = sum % 10;
            }
        }

        answer2 = getFirstDigits(current_digits, 8);
    }

    return { answer1, answer2 };
}

// 3220854
// 3220854
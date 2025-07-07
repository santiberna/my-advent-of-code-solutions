#include "utility/math.hpp"
#include "utility/timer.hpp"
#include <cstdint>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <array>

constexpr size_t NUM_SIZE = 6;
using DigitArray = std::array<uint8_t, NUM_SIZE>;

DigitArray number_to_digits(int64_t num)
{
    return {
        (uint8_t)((num / 100'000) % 10),
        (uint8_t)((num / 10'000) % 10),
        (uint8_t)((num / 1'000) % 10),
        (uint8_t)((num / 100) % 10),
        (uint8_t)((num / 10) % 10),
        (uint8_t)(num % 10),
    };
}

REGISTER_CHALLENGE(Y2019_C4, "372304-847060")
{
    auto range = TextHelpers::ParseAllNumbers<int64_t>(input);

    auto check_increase = [](const DigitArray& digits) -> std::pair<int, int>
    {
        for (size_t i = 1; i < digits.size(); i++)
        {
            if (digits.at(i - 1) > digits.at(i))
            {
                return { static_cast<int>(i), digits.at(i - 1) - digits.at(i) };
            }
        }
        return { 0, 0 };
    };

    auto check_double = [](const DigitArray& digits)
    {
        for (size_t i = 0; i < digits.size() - 1; i++)
        {
            if (digits.at(i) == digits.at(i + 1))
            {
                return true;
            }
        }
        return false;
    };

    auto check_only_double = [](const DigitArray& digits)
    {
        for (int i = 0; i < digits.size() - 1; i++)
        {
            if (digits.at(i) == digits.at(i + 1))
            {
                bool is_first = i == 0;
                bool has_next = i + 2 < digits.size();

                bool same_prev = !is_first && digits.at(i - 1) == digits.at(i);
                bool same_after = has_next && digits.at(i + 2) == digits.at(i);

                if (!same_prev && !same_after)
                {
                    return true;
                }
            }
        }
        return false;
    };

    int64_t answer1 = 0, answer2 = 0;

    for (int64_t pass = range.front(); pass <= range.back(); pass++)
    {
        auto digits = number_to_digits(pass);

        auto [increase, diff] = check_increase(digits);

        if (increase == 5)
        {
            pass += diff;
        }

        if (increase != 0)
        {
            auto digit_exp = math::powInteger<size_t>(10, NUM_SIZE - increase - 1);
            auto div = pass / digit_exp;
            pass = (div + diff) * digit_exp * 10;
        }

        if (!check_double(digits))
            continue;

        answer1++;

        if (check_only_double(digits))
        {
            answer2++;
        }
    }

    return { answer1, answer2 };
}

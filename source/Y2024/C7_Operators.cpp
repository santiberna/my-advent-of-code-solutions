#include <Utility/Math.hpp>
#include <string>
#include <Utility/TextHelpers.hpp>
#include <common.hpp>
#include <vector>
#include <cassert>
#include <bit>
#include <Utility/Log.hpp>
#include <Utility/Timer.hpp>

struct Values
{
    int64_t result {};
    std::vector<int64_t> operands {};
};

void GetPossibilities(int64_t& out, int64_t target, const std::vector<int64_t>& operators_left, bool enable_concat)
{
    assert(operators_left.size() >= 1);
    if (operators_left.size() == 1)
    {
        if (operators_left.front() == target)
            out++;
        return;
    }

    auto last_one = operators_left[0];
    auto last_two = operators_left[1];

    {
        auto mul_result = last_one * last_two;
        auto copy = operators_left;
        copy.erase(copy.begin());
        copy.front() = mul_result;

        GetPossibilities(out, target, copy, enable_concat);

        if (out)
            return;
    }

    {
        auto add_result = last_one + last_two;
        auto copy = operators_left;
        copy.erase(copy.begin());
        copy.front() = add_result;

        GetPossibilities(out, target, copy, enable_concat);

        if (out)
            return;
    }

    if (enable_concat)
    {
        auto digits = NumDigits(last_two);
        auto shifted = last_one * PowInt(10, digits);
        auto num2 = shifted + last_two;

        auto copy = operators_left;
        copy.erase(copy.begin());
        copy.front() = num2;

        GetPossibilities(out, target, copy, enable_concat);

        if (out)
            return;
    }
}

REGISTER_CHALLENGE(Y2024_C7_Operators, "input/Y2024/C7.txt")
{
    std::vector<Values> equations {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        for (const auto& l : lines)
        {
            auto numbers = TextHelpers::ParseAllNumbers<int64_t>(l);

            auto first = numbers.front();
            numbers.erase(numbers.begin());
            equations.emplace_back(first, std::move(numbers));
        }
    }

    int64_t answer1 {}, answer2 {};
    std::vector<Values> failed {};

    {
        PROFILE_SCOPE();
        for (size_t i = 0; i < equations.size(); i++)
        {
            auto& v = equations.at(i);

            int64_t p1 {};
            GetPossibilities(p1, v.result, v.operands, false);
            if (p1)
            {
                answer1 += v.result;
            }
            else
            {
                failed.emplace_back(v);
            }
        }
    }

    {
        PROFILE_SCOPE();
        for (auto& v : failed)
        {
            int64_t p2 {};
            GetPossibilities(p2, v.result, v.operands, true);

            if (p2)
            {
                answer2 += v.result;
            }
        }
    }

    return { answer1, answer2 + answer1 };
}
#include "Utility/Log.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <Utility/array_2D.hpp>
#include <Utility/math.hpp>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

void ExpandStones(std::vector<int64_t>& stones)
{
    size_t current_count = stones.size();

    for (size_t i = 0; i < current_count; i++)
    {
        auto& stone_val = stones.at(i);

        if (stone_val == 0)
        {
            stone_val = 1;
        }
        else if (NumDigits(stone_val) % 2 == 0)
        {
            auto str = std::to_string(stone_val);
            auto split = str.substr(0, str.size() / 2);
            auto split2 = str.substr(str.size() / 2);

            stone_val = std::stoll(split);
            stones.emplace_back(std::stoll(split2));
        }
        else
        {
            stone_val *= 2024;
        }
    }
}

std::unordered_map<glm::ivec2, size_t> memo_table {};

struct ExpandResult
{
    std::vector<glm::ivec2> splits {};
};

ExpandResult ExpandSingleStone(int initial, int iterations)
{
    ExpandResult result = {};
    int64_t number = initial;

    for (size_t i = 0; i < iterations; i++)
    {
        if (number == 0)
        {
            number = 1;
        }
        else if (NumDigits(number) % 2 == 0)
        {
            auto str = std::to_string(number);
            auto split = str.substr(0, str.size() / 2);
            auto split2 = str.substr(str.size() / 2);

            number = std::stoll(split);
            result.splits.emplace_back(std::stoll(split2), iterations - i - 1);
        }
        else
        {
            number *= 2024;
        }
    }

    return result;
}

size_t CalculateStone(int initial, int iterations)
{
    auto init = glm::ivec2 { initial, iterations };

    size_t out {};

    if (auto it = memo_table.find(init); it != memo_table.end())
    {
        out += it->second;
    }
    else if (iterations == 0)
    {
        return 1;
    }
    else
    {
        auto result = ExpandSingleStone(init.x, init.y);
        out++;

        for (auto rit = result.splits.rbegin(); rit != result.splits.rend(); ++rit)
        {
            auto val = CalculateStone(rit->x, rit->y);

            out += val;
            memo_table.emplace(*rit, val);
        }
    }

    return out;
}

REGISTER_CHALLENGE_DISABLED(Y2024_CB, "input/Y2024/C11.txt")
{
    std::vector<int64_t> numbers {};

    if (auto file = TextHelpers::OpenFileReadStream(input))
    {
        auto line = TextHelpers::StreamToString(file.value());
        numbers = TextHelpers::ParseAllNumbers<int64_t>(line);
    }

    int64_t answer1 {}, answer2 {};

    auto in_copy = numbers;
    for (size_t i = 0; i < 25; i++)
    {
        ExpandStones(in_copy);
    }
    answer1 = in_copy.size();

    for (auto n : numbers)
    {
        auto result = CalculateStone(n, 75);
        answer2 += result;
    }

    return { answer1, answer2 };
}
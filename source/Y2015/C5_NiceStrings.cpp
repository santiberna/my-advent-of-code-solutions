#include <Utility/TextHelpers.hpp>
#include <common.hpp>
#include <cassert>
#include <cstdint>
#include <string>

REGISTER_CHALLENGE(Y2015_C5_NiceStrings, "input/Y2015/C5.txt")
{
    std::vector<std::string> strings;

    if (auto file = TextHelpers::OpenFileReadStream(input))
    {
        strings = TextHelpers::SplitStreamIntoLines(file.value());
    }

    auto HasThreeVowels = [](const std::string& str)
    {
        const char VOWELS[] = { 'a', 'e', 'i', 'o', 'u' };

        size_t vowel_count = 0;
        for (auto c : str)
        {
            for (auto v : VOWELS)
                if (c == v)
                    vowel_count++;
        }

        if (vowel_count >= 3)
            return true;
        return false;
    };

    auto HasTwiceInARow = [](const std::string& str)
    {
        for (size_t i = 0; i < str.size() - 1; i++)
        {
            if (str.at(i) == str.at(i + 1))
                return true;
        }
        return false;
    };

    auto DoesNotHaveNaughty = [](const std::string& str)
    {
        const char* NAUGHTY_PAIRS[] = {
            "ab", "cd", "pq", "xy"
        };

        for (auto naughty : NAUGHTY_PAIRS)
        {
            if (str.find(naughty) != std::string::npos)
                return false;
        }
        return true;
    };

    auto CheckRepeatingPairs = [](const std::string& str)
    {
        for (size_t i = 0; i < str.size() - 1; i++)
        {
            auto current_pair = std::make_pair(str.at(i), str.at(i + 1));
            for (size_t j = i + 2; j < str.size() - 1; j++)
            {
                auto check_pair = std::make_pair(str.at(j), str.at(j + 1));
                if (current_pair.first == check_pair.first && current_pair.second == check_pair.second)
                {
                    return true;
                }
            }
        }

        return false;
    };

    auto CheckSameInbetween = [](const std::string& str)
    {
        for (size_t i = 0; i < str.size() - 2; i++)
        {
            auto current_next = std::make_pair(str.at(i), str.at(i + 2));
            if (current_next.first == current_next.second)
                return true;
        }

        return false;
    };

    int64_t answer1 = 0, answer2 = 0;
    for (auto& str : strings)
    {
        if (HasThreeVowels(str) && HasTwiceInARow(str) && DoesNotHaveNaughty(str))
            answer1++;

        if (CheckRepeatingPairs(str) && CheckSameInbetween(str))
            answer2++;
    }

    return { answer1, answer2 };
}
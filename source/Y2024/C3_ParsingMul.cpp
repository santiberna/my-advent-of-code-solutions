#include <common.hpp>
#include <Utility/TextHelpers.hpp>
#include <cstddef>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

REGISTER_CHALLENGE_DISABLED(Y2024_C3_ParsingMul, "input/Y2024/C3.txt")
{
    int64_t answer1 {}, answer2 {};

    std::string text;
    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        text = TextHelpers::StreamToString(stream.value());
    }

    std::regex mul_regex { "mul\\(\\d+,\\d+\\)" };

    std::vector<std::pair<bool, int64_t>> multiplications {};

    auto mulbegin = std::sregex_iterator { text.begin(), text.end(), mul_regex };
    auto end = std::sregex_iterator {};

    for (auto it = mulbegin; it != end; ++it)
    {
        std::string match = it->str();
        size_t offset = it->position();

        bool enabled = true;

        std::string_view previous { text.c_str(), offset };
        size_t found_dont = previous.rfind("don't()");
        size_t found_do = previous.rfind("do()");

        if (found_dont == std::string::npos)
        {
            enabled = true;
        }
        else if (found_do == std::string::npos)
        {
            enabled = false;
        }
        else
        {
            enabled = found_do > found_dont;
        }

        auto numbers = TextHelpers::ParseAllNumbers<int64_t>(match);
        multiplications.emplace_back(enabled, numbers.at(0) * numbers.at(1));
    }

    for (auto [enabled, val] : multiplications)
    {
        answer1 += val;
        if (enabled)
            answer2 += val;
    }

    return { answer1, answer2 };
}
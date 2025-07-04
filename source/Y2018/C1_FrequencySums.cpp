#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>

#include <cstdint>
#include <numeric>
#include <unordered_set>
#include <vector>

REGISTER_CHALLENGE_DISABLED(Y2018_C1, "input/Y2018/C1.txt")
{
    std::vector<int64_t> numbers {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto line = TextHelpers::StreamToString(stream.value());
        numbers = TextHelpers::ParseAllNumbers<int64_t>(line);
    }

    int64_t answer1, answer2 {};

    answer1 = std::accumulate(numbers.begin(), numbers.end(), 0);

    std::unordered_set<int64_t> values_reached;

    int64_t accum = 0;

    for (size_t i = 0; /*Forever*/; i = (i + 1) % numbers.size())
    {
        auto result = values_reached.emplace(accum);

        if (!result.second)
        {
            answer2 = accum;
            break;
        }

        accum += numbers[i];
    }

    return { answer1, answer2 };
}
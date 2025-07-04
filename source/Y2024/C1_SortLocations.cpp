#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>

REGISTER_CHALLENGE_DISABLED(Y2024_C1_SortLocations, "input/Y2024/C1.txt")
{
    int64_t answer1 {}, answer2 {};

    std::vector<int64_t> collumn1 {};
    std::vector<int64_t> collumn2 {};

    if (auto read_stream = TextHelpers::OpenFileReadStream(input))
    {
        std::vector<std::string> lines = TextHelpers::SplitStreamIntoLines(read_stream.value());

        for (auto& l : lines)
        {
            auto numbers = TextHelpers::ParseAllNumbers<int64_t>(l);

            collumn1.emplace_back(numbers.at(0));
            collumn2.emplace_back(numbers.at(1));
        }
    }

    assert(collumn1.size() == collumn2.size());

    std::sort(collumn1.begin(), collumn1.end());
    std::sort(collumn2.begin(), collumn2.end());

    for (size_t i = 0; i < collumn1.size(); i++)
    {
        int64_t diff = std::abs(collumn1.at(i) - collumn2.at(i));
        answer1 += diff;
    }

    for (auto n1 : collumn1)
    {
        int64_t occurrences = 0;

        for (auto n2 : collumn2)
        {
            if (n1 == n2)
                occurrences++;
        }

        answer2 += (occurrences * n1);
    }

    return { answer1, answer2 };
}
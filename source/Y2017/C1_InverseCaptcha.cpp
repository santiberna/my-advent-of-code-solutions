#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>

#include <vector>

REGISTER_CHALLENGE_DISABLED(Y2017_C1, "input/Y2017/C1.txt")
{
    std::vector<int64_t> numbers {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto line = TextHelpers::StreamToString(stream.value());

        for (auto c : line)
        {
            numbers.emplace_back(c - '0');
        }
    }

    int64_t answer1 {}, answer2 {};

    for (size_t i = 0; i < numbers.size(); i++)
    {
        auto current = numbers[i];
        auto next = numbers[(i + 1) % numbers.size()];

        auto halfway = (i + numbers.size() / 2) % numbers.size();
        auto halfway_val = numbers[halfway];

        if (current == next)
        {
            answer1 += current;
        }

        if (current == halfway_val)
        {
            answer2 += current;
        }
    }

    return { answer1, answer2 };
}
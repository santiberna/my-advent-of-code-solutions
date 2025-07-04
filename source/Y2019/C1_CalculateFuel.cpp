#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>

REGISTER_CHALLENGE(Y2019_C1, "input/Y2019/C1.txt")
{
    std::vector<int64_t> numbers { 100756 };

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto line = TextHelpers::StreamToString(stream.value());
        numbers = TextHelpers::ParseAllNumbers<int64_t>(line);
    }

    int64_t answer1 {}, answer2 {};

    auto calc_fuel = [](int64_t a)
    { return a / 3 - 2; };

    for (auto num : numbers)
    {
        auto fuel = calc_fuel(num);
        answer1 += fuel;

        answer2 += fuel;
        for (int64_t extra = calc_fuel(fuel); extra > 0; extra = calc_fuel(extra))
        {
            answer2 += extra;
        }
    }

    return { answer1, answer2 };
}

// 3220854
// 3220854
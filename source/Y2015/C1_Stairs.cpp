#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>

REGISTER_CHALLENGE(Y2015_C1_Stairs, "input/Y2015/C1.txt")
{
    std::string line {};
    if (auto file = TextHelpers::OpenFileReadStream(input))
    {
        line = TextHelpers::StreamToString(file.value());
    }

    int64_t answer1 = 0;

    bool set2 = false;
    int64_t answer2 = 0;

    for (size_t i = 0; i < line.length(); i++)
    {
        auto c = line.at(i);
        if (c == '(')
            answer1++;
        if (c == ')')
            answer1--;

        if (!set2 && answer1 == -1)
        {
            set2 = true;
            answer2 = static_cast<int64_t>(i);
        }
    }

    return { answer1, answer2 };
}
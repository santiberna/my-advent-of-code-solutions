#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>

#include <algorithm>
#include <cstdint>

REGISTER_CHALLENGE(Y2015_C2_WrappingPaper, "input/Y2015/C2.txt")
{
    std::vector<std::string> lines {};
    if (auto file = TextHelpers::OpenFileReadStream(input))
    {
        lines = TextHelpers::SplitStreamIntoLines(file.value());
    }

    struct Present
    {
        int64_t x {}, y {}, z {};
    };

    std::vector<Present> presents_to_wrap {};

    for (const auto& line : lines)
    {
        auto numbers = TextHelpers::ParseAllNumbers<int64_t>(line);
        std::sort(numbers.begin(), numbers.end());
        presents_to_wrap.emplace_back(numbers[0], numbers[1], numbers[2]);
    }

    auto GetSmallestFaceArea = [](const Present& p)
    { return p.x * p.y; };

    auto GetSurfaceArea = [](const Present& p)
    { return 2 * p.x * p.y + 2 * p.y * p.z + 2 * p.x * p.z; };

    auto GetSmallestPerimeter = [](const Present& p)
    { return 2 * p.x + 2 * p.y; };

    auto GetVolume = [](const Present& p)
    { return p.x * p.y * p.z; };

    int64_t answer1 = 0, answer2 = 0;

    for (const auto& p : presents_to_wrap)
    {
        answer1 += GetSmallestFaceArea(p) + GetSurfaceArea(p);
        answer2 += GetSmallestPerimeter(p) + GetVolume(p);
    }

    return { answer1, answer2 };
}
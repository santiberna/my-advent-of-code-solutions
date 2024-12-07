#include <common.hpp>
#include <Utility/TextHelpers.hpp>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <vector>

REGISTER_CHALLENGE(Y2024_C2_SafeReports, "input/Y2024/C2.txt")
{
    int64_t answer1 {}, answer2 {};

    std::vector<std::vector<int>> reports {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        for (auto& l : lines)
        {
            reports.emplace_back(TextHelpers::ParseAllNumbers<int>(l));
        }
    }

    auto evaluate_report = [](const std::vector<int>& report)
    {
        bool increasing = report.at(1) > report.at(0);

        for (size_t i = 0; i < report.size() - 1; i++)
        {
            auto first = report.at(i);
            auto second = report.at(i + 1);

            if ((second > first) != increasing)
            {
                return false;
            }

            auto diff = std::abs(first - second);
            if (diff < 1 || diff > 3)
            {
                return false;
            }
        }

        return true;
    };

    for (auto& report : reports)
    {
        if (evaluate_report(report))
        {
            answer1++;
        }

        for (size_t i = 0; i < report.size(); i++)
        {
            auto copy = report;
            copy.erase(copy.begin() + static_cast<ptrdiff_t>(i));

            if (evaluate_report(copy))
            {
                answer2++;
                break;
            }
        }
    }

    return { answer1, answer2 };
}
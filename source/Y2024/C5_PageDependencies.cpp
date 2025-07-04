#include <glm/glm.hpp>
#include <common.hpp>
#include <Utility/TextHelpers.hpp>
#include <Utility/Log.hpp>
#include <unordered_set>

REGISTER_CHALLENGE_DISABLED(Y2024_C5_PageDependencies, "input/Y2024/C5.txt")
{
    std::unordered_map<int64_t, std::unordered_set<int64_t>> page_dependencies {};
    std::vector<std::vector<int64_t>> pages_instructions {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        bool at_header = true;
        for (auto line : lines)
        {

            if (line.empty())
            {
                at_header = false;
                continue;
            }

            auto numbers = TextHelpers::ParseAllNumbers<int64_t>(line);
            if (at_header)
            {
                page_dependencies[numbers[0]].emplace(numbers[1]);
            }
            else
            {
                pages_instructions.emplace_back(numbers);
            }
        }
    }

    int64_t answer1 {}, answer2 {};
    std::vector<std::vector<int64_t>> correct_updates {};
    std::vector<std::vector<int64_t>> incorrect_updates {};

    auto process_update = [&page_dependencies](const std::vector<int64_t>& line)
    {
        for (size_t i = 0; i < line.size(); i++)
        {
            auto n = line[i];

            for (size_t j = 0; j < i; j++)
            {
                auto m = line[j];
                bool contains = page_dependencies[n].contains(m);
                if (contains)
                {
                    return false;
                }
            }
        }

        return true;
    };

    for (const auto& u : pages_instructions)
    {
        if (process_update(u))
        {
            correct_updates.emplace_back(u);
        }
        else
        {
            incorrect_updates.emplace_back(u);
        }
    }

    auto sort_update = [&page_dependencies](std::vector<int64_t>& line)
    {
        for (size_t i = 0; i < line.size(); i++)
        {
            auto n = line[i];

            for (size_t j = 0; j < i; j++)
            {
                auto m = line[j];
                bool contains = page_dependencies[n].contains(m);

                if (contains)
                {
                    std::swap(line[i], line[j]);
                }
            }
        }
    };

    for (auto& r : correct_updates)
    {
        size_t half = ((r.size() + 1) / 2) - 1;
        answer1 += r.at(half);
    }

    for (auto& incorrect : incorrect_updates)
    {
        sort_update(incorrect);
        size_t half = ((incorrect.size() + 1) / 2) - 1;
        answer2 += incorrect.at(half);
    }

    return { answer1, answer2 };
}
#include "utility/log.hpp"
#include "utility/array_2d.hpp"
#include <algorithm>
#include <cstdlib>
#include <istream>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <utility/math.hpp>
#include <sstream>
#include <vector>

std::vector<IVec2> parse_line(std::istream& line)
{
    std::vector<IVec2> ret;
    auto lines = TextHelpers::SplitStreamIntoLines(line, ',');

    for (auto& line : lines)
    {
        auto first = line.front();
        auto num = TextHelpers::ParseNumber<int>({ line.begin() + 1, line.end() }).value().first;
        IVec2 out {};

        switch (first)
        {
        case 'R':
            out.x = +num;
            break;
        case 'L':
            out.x = -num;
            break;
        case 'U':
            out.y = -num;
            break;
        case 'D':
            out.y = +num;
            break;
        default:
            throw;
        }

        ret.emplace_back(out);
    }

    return ret;
}

REGISTER_CHALLENGE(Y2019_C3, "input/Y2019/C3.txt")
{
    std::vector<IVec2> line1 {};
    std::vector<IVec2> line2 {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        std::stringstream first {};
        first << lines.at(0);

        line1 = parse_line(first);

        std::stringstream second {};
        second << lines.at(1);

        line2 = parse_line(second);
    }

    constexpr int ARRAY_WIDTH = 20000;
    constexpr int ARRAY_HEIGHT = 20000;

    Array2D<int> grid { ARRAY_WIDTH, ARRAY_HEIGHT, 0 };
    auto position = IVec2 { ARRAY_WIDTH / 2, ARRAY_HEIGHT / 2 };
    int steps = 0;

    for (auto direction : line1)
    {
        for (int i = 0; i != direction.x; i += intSign(direction.x))
        {
            steps++;
            position.x += intSign(direction.x);
            *grid(position.x, position.y) = steps;
        }

        for (int j = 0; j != direction.y; j += intSign(direction.y))
        {
            steps++;
            position.y += intSign(direction.y);
            *grid(position.x, position.y) = steps;
        }
    }

    steps = 0;
    position = IVec2 { ARRAY_WIDTH / 2, ARRAY_HEIGHT / 2 };

    auto calc_distance = [](const IVec2& a, const IVec2& b)
    {
        auto distance = a - b;
        return std::labs(distance.x) + std::labs(distance.y);
    };

    std::vector<int> intersection_distances;
    std::vector<int> intersection_steps;

    for (auto direction : line2)
    {
        for (int i = 0; i != direction.x; i += intSign(direction.x))
        {
            steps++;
            position.x += intSign(direction.x);

            if (*grid(position.x, position.y) != 0)
            {
                intersection_distances.emplace_back(calc_distance(IVec2 { ARRAY_WIDTH / 2, ARRAY_HEIGHT / 2 }, position));
                intersection_steps.emplace_back(steps + *grid(position.x, position.y));
            }
        }

        for (int j = 0; j != direction.y; j += intSign(direction.y))
        {
            steps++;
            position.y += intSign(direction.y);

            if (*grid(position.x, position.y) != 0)
            {
                intersection_distances.emplace_back(calc_distance(IVec2 { ARRAY_WIDTH / 2, ARRAY_HEIGHT / 2 }, position));
                intersection_steps.emplace_back(steps + *grid(position.x, position.y));
            }
        }
    }

    int64_t answer1 = *std::min_element(intersection_distances.begin(), intersection_distances.end());
    int64_t answer2 = *std::min_element(intersection_steps.begin(), intersection_steps.end());

    return { answer1, answer2 };
}
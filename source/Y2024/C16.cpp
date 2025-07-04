#include "Utility/Array2D.hpp"
#include "Utility/Log.hpp"
#include "glm/fwd.hpp"
#include <Utility/Math.hpp>
#include <algorithm>
#include <array>
#include <common.hpp>
#include <Utility/TextHelpers.hpp>
#include <limits.h>
#include <ostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int64_t Traverse(Array2D<char>& grid, glm::ivec2 start, glm::ivec2 start_dir)
{

    struct Step
    {
        glm::ivec2 pos;
        glm::ivec2 dir;
        int64_t current_score;

        bool operator<(const Step& s) const
        {
            return current_score > s.current_score;
        }
    };

    std::unordered_set<glm::ivec2> visited {};
    std::priority_queue<Step> next_steps {};
    next_steps.push(Step { start, start_dir, 0 });

    while (next_steps.size())
    {
        auto step = next_steps.top();
        next_steps.pop();

        while (*grid(step.pos.x, step.pos.y) != '#')
        {
            if (*grid(step.pos.x, step.pos.y) == 'E')
            {
                return step.current_score;
            }

            auto left = step.pos + TurnVec2(step.dir, true);
            auto right = step.pos + TurnVec2(step.dir, false);

            if (*grid(left.x, left.y) == '.')
            {
                next_steps.push(
                    Step { left, TurnVec2(step.dir, true), step.current_score + 1001 });
            }

            if (*grid(right.x, right.y) == '.')
            {
                next_steps.push(
                    Step { right, TurnVec2(step.dir, false), step.current_score + 1001 });
            }

            step.pos += step.dir;
            step.current_score += 1;
        }
    }

    throw;
}

REGISTER_CHALLENGE_DISABLED(Y2024_CG, "input/Y2024/C16.txt")
{
    Array2D<char> map {};

    glm::ivec2 start {};
    glm::ivec2 dir = { 1, 0 };

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        size_t width = lines.front().size();
        size_t height = lines.size();
        map = Array2D<char>(width, height);

        for (size_t j = 0; j < height; j++)
        {
            for (size_t i = 0; i < width; i++)
            {
                if (lines[j][i] == 'S')
                {
                    start = { i, j };
                }

                *map(i, j) = lines[j][i];
            }
        }
    }

    int64_t answer1 {}, answer2 {};

    answer1 = Traverse(map, start, dir);

    // for (size_t j = 0; j < map.GetHeight(); j++)
    // {
    //     for (size_t i = 0; i < map.GetWidth(); i++)
    //     {
    //         if (*map(i, j) == 'E')
    //         {
    //             answer1 = *scores(i, j);
    //         }
    //     }
    // }

    return { answer1, answer2 };
}
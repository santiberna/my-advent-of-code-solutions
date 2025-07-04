#include "utility/array_2D.hpp"
#include "utility/challenge_runner.hpp"

#include <utility/text_helpers.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <cstdint>

enum PathDirection : uint8_t
{
    UP = 1,
    RIGHT = 2,
    DOWN = 3,
    LEFT = 4
};

PathDirection GetMark(const glm::ivec2& v)
{
    if (v.x == 1)
    {
        return RIGHT;
    }
    if (v.x == -1)
    {
        return LEFT;
    }
    if (v.y == 1)
    {
        return DOWN;
    }
    if (v.y == -1)
    {
        return UP;
    }
}

glm::ivec2 GetDir(PathDirection d)
{
    switch (d)
    {
    case UP:
        return { 0, -1 };
        break;
    case RIGHT:
        return { 1, 0 };
        break;
    case DOWN:
        return { 0, 1 };
        break;
    case LEFT:
        return { -1, 0 };
        break;
    default:
        throw;
        break;
    }
}

void PrintPath(const Array2D<uint8_t>& path)
{
    for (size_t j = 0; j < path.GetHeight(); j++)
    {
        for (size_t i = 0; i < path.GetWidth(); i++)
        {
            switch (*path(i, j))
            {
            case UP:
                std::cout << "^";
                break;
            case RIGHT:
                std::cout << ">";
                break;
            case DOWN:
                std::cout << "v";
                break;
            case LEFT:
                std::cout << "<";
                break;
            default:
                std::cout << ".";
                break;
            }
        }
        std::cout << std::endl;
    }
}

bool Traverse(Array2D<uint8_t>& path, const Array2D<uint8_t>& obstacles, glm::ivec2 start, glm::ivec2 direction)
{
    size_t step_count = obstacles.GetWidth() * obstacles.GetHeight();
    auto mark = GetMark(direction);

    while (--step_count)
    {
        auto next_step = start + direction;

        auto p = obstacles(next_step.x, next_step.y);

        if (p == nullptr)
        {
            // Log::Debug("{}", step_count);
            break;
        }

        glm::ivec2 rotated {};
        rotated.x = -direction.y;
        rotated.y = +direction.x;

        if (*p == 1)
        {
            direction = rotated;

            mark = GetMark(direction);
            *path(start.x, start.y) = mark;
            continue;
        }

        // if (*path(next_step.x, next_step.y) == mark)
        // {
        //     return true;
        // }

        start = next_step;
        *path(start.x, start.y) = mark;
    }

    if (step_count == 0)
    {
        // PrintPath(path);
        return true;
    }

    // PrintPath(path);
    return false;
}

REGISTER_CHALLENGE_DISABLED(Y2024_C6_GuardTracking, "input/Y2024/C6.txt")
{
    Array2D<uint8_t> obstacles {};
    Array2D<uint8_t> path {};
    Array2D<uint8_t> placed {};

    glm::ivec2 direction { 0, -1 };
    PathDirection mark = UP;
    glm::ivec2 start {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        size_t width = lines.front().size();
        size_t height = lines.size();

        obstacles = Array2D<uint8_t>(width, height);
        path = Array2D<uint8_t>(width, height);
        placed = Array2D<uint8_t>(width, height);

        for (size_t j = 0; j < height; j++)
        {
            for (size_t i = 0; i < width; i++)
            {

                auto c = lines[j][i];

                if (c == '^')
                {
                    start = { i, j };
                }
                else if (c == '#')
                {
                    *obstacles(i, j) = 1;
                }
            }
        }
    }

    int64_t answer1 {}, answer2 {};

    *path(start.x, start.y) = 1;
    Traverse(path, obstacles, start, direction);

    for (size_t j = 0; j < path.GetHeight(); j++)
    {
        for (size_t i = 0; i < path.GetWidth(); i++)
        {
            if (*obstacles(i, j) != 0)
                continue;

            auto new_obstacles = obstacles;
            auto new_path = Array2D<uint8_t>(path.GetWidth(), path.GetHeight());
            *new_obstacles(i, j) = 1;

            if (Traverse(new_path, new_obstacles, start, direction))
            {
                *placed(i, j) = 1;
            }
        }
    }

    for (size_t j = 0; j < path.GetHeight(); j++)
    {
        for (size_t i = 0; i < path.GetWidth(); i++)
        {
            if (*path(i, j))
                answer1++;
            if (*placed(i, j))
                answer2++;
        }
    }

    // PrintPath(path);
    // std::endl(std::cout);
    // PrintPath(placed);
    return { answer1, answer2 };
}
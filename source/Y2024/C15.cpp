#include <utility/array_2D.hpp>
#include <cassert>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <queue>
#include <vector>
#include <glm/glm.hpp>

bool PushBox(Array2D<char>& grid, glm::ivec2 pos, glm::ivec2 dir)
{
    auto next_pos = pos + dir;
    auto next_tile = *grid(next_pos.x, next_pos.y);

    if (next_tile == '.')
    {
        return true;
    }
    else if (next_tile == '#')
        return false;
    else if (next_tile == 'O')
    {
        auto can_push = PushBox(grid, next_pos, dir);

        if (can_push)
        {
            *grid(next_pos.x, next_pos.y) = '.';
            *grid(next_pos.x + dir.x, next_pos.y + dir.y) = 'O';
            return true;
        }
    }
    return false;
}

bool PushBox2(Array2D<char>& grid, glm::ivec2 pos, glm::ivec2 dir)
{
    auto next_pos = pos + dir;
    auto next_tile = *grid(next_pos.x, next_pos.y);

    if (next_tile == '.')
    {
        return true;
    }
    else if (next_tile == '#')
    {
        return false;
    }
    else if (next_tile == ']' || next_tile == '[')
    {
        if (dir.y == 0)
        {
            auto can_push = PushBox2(grid, next_pos, dir);

            if (can_push)
            {
                *grid(next_pos.x, next_pos.y) = '.';
                *grid(next_pos.x + dir.x, next_pos.y + dir.y) = next_tile;
                return true;
            }
        }
        else
        {
            auto incr = next_tile == '[' ? glm::ivec2(1, 0) : glm::ivec2(-1, 0);
            auto other_val = next_tile == '[' ? ']' : '[';

            auto other_side = next_pos + incr;
            assert(*grid(other_side.x, other_side.y) == '[' || *grid(other_side.x, other_side.y) == ']');

            auto push1 = PushBox2(grid, next_pos, dir);
            auto push2 = PushBox2(grid, other_side, dir);

            if (push1 && push2)
            {
                *grid(next_pos.x, next_pos.y) = '.';
                *grid(next_pos.x + dir.x, next_pos.y + dir.y) = next_tile;
                *grid(other_side.x, other_side.y) = '.';
                *grid(other_side.x + dir.x, other_side.y + dir.y) = other_val;
                return true;
            }
        }
    }

    return false;
}

REGISTER_CHALLENGE_DISABLED(Y2024_CF, "input/Y2024/C15.txt")
{
    glm::ivec2 initial_pos {};
    std::vector<glm::ivec2> steps {};
    Array2D<char> map {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());
        bool parsing_instructions = false;

        for (size_t i = 0; i < lines.size(); i++)
        {
            auto& l = lines[i];

            if (parsing_instructions)
            {
                for (auto c : l)
                {
                    switch (c)
                    {
                    case '>':
                        steps.emplace_back(1, 0);
                        break;
                    case '<':
                        steps.emplace_back(-1, 0);
                        break;
                    case '^':
                        steps.emplace_back(0, -1);
                        break;
                    case 'v':
                        steps.emplace_back(0, 1);
                        break;
                    default:
                        break;
                    }
                }
            }

            if (l.empty() && !parsing_instructions)
            {
                parsing_instructions = true;
                size_t width = lines.front().size();
                size_t height = i;

                map = Array2D<char>(width, height);

                for (size_t j = 0; j < height; j++)
                {
                    for (size_t k = 0; k < width; k++)
                    {
                        auto c = lines[j][k];
                        if (c == '@')
                        {
                            initial_pos = { k, j };
                            *map(k, j) = '.';
                        }
                        else
                        {
                            *map(k, j) = c;
                        }
                    }
                }
            }
        }
    }

    // Generate Bigger map

    glm::ivec2 pos2 = { initial_pos.x * 2, initial_pos.y };
    Array2D<char> map2 { map.GetWidth() * 2, map.GetHeight() };

    for (size_t j = 0; j < map.GetHeight(); j++)
    {
        for (size_t i = 0; i < map.GetWidth(); i++)
        {
            auto c = *map(i, j);

            if (c == '.')
            {
                *map2(i * 2, j) = '.';
                *map2(i * 2 + 1, j) = '.';
            }

            if (c == 'O')
            {
                *map2(i * 2, j) = '[';
                *map2(i * 2 + 1, j) = ']';
            }

            if (c == '#')
            {
                *map2(i * 2, j) = '#';
                *map2(i * 2 + 1, j) = '#';
            }
        }
    }

    auto map1 = map;

    for (auto step : steps)
    {
        auto res = PushBox(map1, initial_pos, step);
        if (res)
        {
            initial_pos += step;
        }
    }

    int i = 0;
    for (auto step : steps)
    {
        auto copy = map2;

        auto res = PushBox2(copy, pos2, step);
        if (res)
        {
            pos2 += step;
            map2 = copy;
        }
    }

    int64_t answer1 {}, answer2 {};

    for (size_t j = 0; j < map1.GetHeight(); j++)
    {
        for (size_t i = 0; i < map.GetWidth(); i++)
        {
            auto c = *map1(i, j);

            if (c == 'O')
            {
                answer1 += (j * 100) + i;
            }
        }
    }

    for (size_t j = 0; j < map2.GetHeight(); j++)
    {
        for (size_t i = 0; i < map2.GetWidth(); i++)
        {
            auto c = *map2(i, j);

            if (c == '[')
            {
                answer2 += (j * 100) + i;
            }
        }
    }

    return { answer1, answer2 };
}
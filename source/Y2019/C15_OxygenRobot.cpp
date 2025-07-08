#include "utility/array_2d.hpp"
#include "utility/math.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <queue>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <utility/int_code.hpp>
#include <vector>

using IVec2 = math::Vec2<int64_t>;

enum Direction : int64_t
{
    NORTH = 1,
    SOUTH = 2,
    WEST = 3,
    EAST = 4
};

IVec2 getDir(Direction dir)
{
    switch (dir)
    {
    case Direction::NORTH:
        return { 0, -1 };
    case Direction::EAST:
        return { 1, 0 };
    case Direction::SOUTH:
        return { 0, 1 };
    case Direction::WEST:
        return { -1, 0 };
    }
}

Direction oppositeDir(Direction dir)
{
    switch (dir)
    {
    case Direction::NORTH:
        return Direction::SOUTH;
    case Direction::EAST:
        return Direction::WEST;
    case Direction::SOUTH:
        return Direction::NORTH;
    case Direction::WEST:
        return Direction::EAST;
    }
}

int64_t checkPosition(ic::Program& program, Direction dir)
{
    program.inputs.push(dir);
    auto result = program.run();
    assert(result == ic::WAITING_INPUT);

    auto detected = program.outputs.back();
    program.outputs.clear();

    return detected;
}

REGISTER_CHALLENGE(Y2019_CF, "input/Y2019/C15.txt")
{
    ic::Program program;

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto line = TextHelpers::StreamToString(stream.value());
        program.state = TextHelpers::ParseAllNumbers<int64_t>(line);
    }

    program.state.resize(1024 * 256);

    int64_t answer1 {};
    int64_t answer2 {};

    constexpr size_t WIDTH = 42;
    constexpr size_t HEIGHT = 42;

    Array2D<char> map { WIDTH, HEIGHT, ' ' };
    IVec2 robot_pos = { WIDTH / 2, HEIGHT / 2 };

    map.at(robot_pos.x, robot_pos.y) = '.';
    std::stack<Direction> prev_steps {};

    constexpr Direction ORDER[4] = { NORTH, EAST, SOUTH, WEST };

    while (true)
    {
        bool moved = false;

        for (auto d : ORDER)
        {
            auto position = robot_pos + getDir(d);

            if (map.at(position.x, position.y) != ' ')
            {
                continue;
            }

            auto result = checkPosition(program, d);

            if (result == 0)
            {
                map.at(position.x, position.y) = '#';
            }
            else if (result == 1)
            {
                map.at(position.x, position.y) = '.';
                prev_steps.push(oppositeDir(d));
                moved = true;
                break;
            }
            else if (result == 2)
            {
                map.at(position.x, position.y) = 'O';
                prev_steps.push(oppositeDir(d));
                moved = true;
                break;
            }
            else
                throw;
        }

        if (moved)
        {
            robot_pos += getDir(oppositeDir(prev_steps.top()));
        }
        else
        {
            if (prev_steps.size() == 0)
            {
                break;
            }

            auto prev = prev_steps.top();
            prev_steps.pop();

            auto result = checkPosition(program, prev);
            assert(result != ' ' && result != '#');

            robot_pos += getDir(prev);
        }
    }

    { // Part 1
        Array2D<int> visited { WIDTH, HEIGHT, -1 };
        IVec2 initial_pos = { WIDTH / 2, HEIGHT / 2 };
        IVec2 target {};

        for (auto it = visited.begin(); it != visited.end(); ++it)
        {
            auto [x, y] = it.getPosition();

            if (map.at(x, y) == '#')
            {
                *it = -2;
            }

            if (map.at(x, y) == 'O')
            {
                target = IVec2(x, y);
            }
        }

        visited.at(initial_pos.x, initial_pos.y) = 0;

        std::queue<IVec2> to_check;
        to_check.push(initial_pos);

        while (!to_check.empty())
        {
            auto next = to_check.front();
            to_check.pop();

            auto cost = visited.at(next.x, next.y) + 1;

            for (auto d : ORDER)
            {
                auto new_pos = next + getDir(d);

                if (visited.at(new_pos.x, new_pos.y) == -1)
                {
                    visited.at(new_pos.x, new_pos.y) = cost;
                    to_check.push(new_pos);
                }
            }
        }

        answer1 = visited.at(target.x, target.y);
    }

    { // Part 2

        Array2D<int> visited { WIDTH, HEIGHT, -1 };
        IVec2 oxygen {};

        for (auto it = visited.begin(); it != visited.end(); ++it)
        {
            auto [x, y] = it.getPosition();

            if (map.at(x, y) == '#')
            {
                *it = -2;
            }

            if (map.at(x, y) == 'O')
            {
                oxygen = IVec2(x, y);
            }
        }

        visited.at(oxygen.x, oxygen.y) = 0;

        std::queue<IVec2> to_check;
        to_check.push(oxygen);

        while (!to_check.empty())
        {
            auto next = to_check.front();
            to_check.pop();

            auto cost = visited.at(next.x, next.y) + 1;

            for (auto d : ORDER)
            {
                auto new_pos = next + getDir(d);

                if (visited.at(new_pos.x, new_pos.y) == -1)
                {
                    visited.at(new_pos.x, new_pos.y) = cost;
                    to_check.push(new_pos);
                }
            }
        }

        for (auto i : visited)
        {
            answer2 = std::max<int64_t>(answer2, i);
        }
    }

    return { answer1, answer2 };
}
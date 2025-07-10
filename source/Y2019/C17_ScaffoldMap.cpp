#include "utility/array_2d.hpp"
#include "utility/math.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <coroutine>
#include <iostream>
#include <thread>
#include <utility/challenge_runner.hpp>
#include <utility/int_code.hpp>
#include <utility/text_helpers.hpp>

using IVec2 = math::Vec2<int64_t>;

Array2D<char> getView(const std::vector<int64_t>& output)
{
    Array2D<char> map { 48, 48 };

    size_t i = 0;
    size_t j = 0;

    for (size_t k = 0; k < output.size(); ++k)
    {
        auto c = output.at(k);

        if (c == 10)
        {
            j++;
            i = 0;
        }
        else
        {
            map.at(i, j) = c;
            i++;
        }
    }

    return map;
}

void input_string(ic::Program& program, const std::string& string)
{
    for (auto& c : string)
    {
        program.inputs.push(c);
    }

    program.inputs.push(10);
}

std::string extract_output(ic::Program& program)
{
    std::string out;

    for (auto& c : program.outputs)
    {
        out.push_back(c);
    }

    program.outputs.clear();
    return out;
}

[[maybe_unused]] constexpr auto WALK_SOLUTION = "L,10,L,12,R,6,R,10,L,4,L,4,L,12,L,10,L,12,R,6,R,10,L,4,L,4,L,12,L,10,L,12,R,6,L,10,R,10,R,6,L,4,R,10,L,4,L,4,L,12,L,10,R,10,R,6,L,4,L,10,L,12,R,6,L,10,R,10,R,6,L,4";

REGISTER_CHALLENGE(Y2019_CH, "input/Y2019/C17.txt")
{
    ic::Program program = ic::Program::fromFile(input).value();

    int64_t answer1 {}, answer2;

    Array2D<char> map { 48, 48 };

    {
        auto map_view = program;
        map_view.run();

        map = getView(map_view.outputs);

        std::vector<IVec2> intersections {};

        for (auto it = map.begin(); it != map.end(); ++it)
        {
            if (*it != '#')
            {
                continue;
            }

            auto [x, y] = it.getPosition();

            if (x == 0 || y == 0)
            {
                continue;
            }

            if (x == map.getWidth() - 1 || y == map.getHeight() - 1)
            {
                continue;
            }

            auto n = map.at(x, y - 1);
            auto e = map.at(x + 1, y);
            auto w = map.at(x - 1, y);
            auto s = map.at(x, y + 1);

            if (n == '#' && e == '#' && w == '#' && s == '#')
            {
                intersections.emplace_back((int64_t)x, (int64_t)y);
            }
        }

        for (auto i : intersections)
        {
            map.at(i.x, i.y) = 'O';
            auto res = i.x * i.y;
            answer1 += res;
        }
    }

    {
        auto robot_sim = program;
        robot_sim.state.front() = 2;

        robot_sim.run();

        std::vector<std::string> inputs {
            "A,B,A,B,A,C,B,C,A,C",
            "L,10,L,12,R,6",
            "R,10,L,4,L,4,L,12",
            "L,10,R,10,R,6,L,4",
        };

        for (auto& in : inputs)
        {
            assert(in.size() <= 20);
            input_string(robot_sim, in);

            robot_sim.run();
        }

        input_string(robot_sim, "n");
        robot_sim.run();

        answer2 = robot_sim.outputs.back();
    }

    return { answer1, answer2 };
}
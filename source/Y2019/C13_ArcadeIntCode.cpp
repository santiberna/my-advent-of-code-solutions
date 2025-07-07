#include "utility/array_2d.hpp"
#include "utility/math.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <utility/int_code.hpp>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>

using IVec2 = math::Vec2<int64_t>;

REGISTER_CHALLENGE(Y2019_CD, "input/Y2019/C13.txt")
{
    ic::Program program;

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto line = TextHelpers::StreamToString(stream.value());
        program.state = TextHelpers::ParseAllNumbers<int64_t>(line);
    }

    program.state.resize(1024 * 256);

    int64_t answer1 = 0;
    int64_t answer2 = 0;

    {
        auto test_run = program;
        test_run.run();

        Array2D<int64_t> screen { 43, 22 };

        for (size_t i = 0; i < test_run.outputs.size(); i += 3)
        {
            auto x = test_run.outputs[i];
            auto y = test_run.outputs[i + 1];
            auto id = test_run.outputs[i + 2];

            screen.at(x, y) = id;
        }

        for (auto id : screen)
        {
            if (id == 2)
                answer1++;
        }
    }

    {
        auto actual_run = program;
        actual_run.state.at(0) = 2;

        int64_t score = -1;
        IVec2 ball_pos = {};
        IVec2 paddle_pos = {};

        auto result = ic::ExitCode::WAITING_INPUT;

        while (result != ic::ExitCode::HALT)
        {
            actual_run.outputs.clear();
            result = actual_run.run();

            for (size_t i = 0; i < actual_run.outputs.size(); i += 3)
            {
                auto x = actual_run.outputs[i];
                auto y = actual_run.outputs[i + 1];
                auto id = actual_run.outputs[i + 2];

                if (x == -1 && y == 0)
                {
                    score = id;
                    continue;
                }

                switch (id)
                {
                case 3:
                    paddle_pos = { x, y };
                    break;
                case 4:
                    ball_pos = { x, y };
                    break;
                default:
                    break;
                }
            }

            auto diff = ball_pos.x - paddle_pos.x;
            auto sign = math::sign(diff);

            actual_run.inputs.push(sign);
        }

        answer2 = score;
    }

    return { answer1, answer2 };
}
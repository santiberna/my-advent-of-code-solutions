#include <utility/int_code.hpp>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <utility/math.hpp>

using IVec2 = math::Vec2<int>;

REGISTER_CHALLENGE(Y2019_CB, "input/Y2019/C11.txt")
{
    ic::Program script;
    script.state = {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto line = TextHelpers::StreamToString(stream.value());
        script.state = TextHelpers::ParseAllNumbers<int64_t>(line);
    }

    script.state.resize(1024 * 10);

    constexpr size_t WIDTH = 150;
    constexpr size_t HEIGHT = 150;

    int64_t answer1 = 0;
    int64_t answer2 = 0;

    {
        auto program = script;

        Array2D<char> map { WIDTH, HEIGHT, 2 };
        IVec2 position { WIDTH / 2, HEIGHT / 2 };
        IVec2 direction { 0, -1 };

        while (true)
        {
            auto colour = map.at(position.x, position.y) == 1;

            program.inputs.push(colour);
            bool end = program.run() == ic::HALT;

            if (end)
            {
                break;
            }

            auto new_dir_code = program.outputs.back();
            program.outputs.pop_back();

            auto paint_colour = program.outputs.back();
            program.outputs.pop_back();

            map.at(position.x, position.y) = (char)paint_colour;

            direction = math::rotate(direction, !(bool)(new_dir_code));

            position += direction;
        }

        for (auto c : map)
        {
            if (c != 2)
            {
                answer1++;
            }
        }
    }

    {
        auto program = script;

        Array2D<char> map { WIDTH, HEIGHT, 2 };
        IVec2 position { WIDTH / 2, HEIGHT / 2 };
        IVec2 direction { 0, -1 };

        map.at(position.x, position.y) = 1;

        while (true)
        {
            auto colour = map.at(position.x, position.y) == 1;

            program.inputs.push(colour);
            bool end = program.run() == ic::HALT;

            if (end)
            {
                break;
            }

            auto new_dir_code = program.outputs.back();
            program.outputs.pop_back();

            auto paint_colour = program.outputs.back();
            program.outputs.pop_back();

            map.at(position.x, position.y) = (char)paint_colour;

            direction = math::rotate(direction, !(bool)(new_dir_code));

            position += direction;
        }

        for (auto& c : map)
        {
            if (c == 1)
            {
                c = '#';
            }
            else
            {
                c = '.';
            }
        }

        // std::cout << map << std::endl;
    }

    return { answer1, answer2 };
}

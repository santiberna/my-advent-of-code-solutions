#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>

#include <utility/math.hpp>
#include <cstdint>
#include <vector>

REGISTER_CHALLENGE(Y2015_C6_LightGrids, "input/Y2015/C6.txt")
{
    int64_t answer1 = 0, answer2 = 0;

    const char* TOGGLE = "toggle";
    const char* TURN_ON = "turn on";
    const char* TURN_OFF = "turn off";

    enum class Type
    {
        TOGGLE,
        TURN_ON,
        TURN_OFF
    };

    struct Instruction
    {
        Type t {};
        glm::ivec2 start {}, end {};
    };

    std::vector<Instruction> steps {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        for (const auto& line : lines)
        {
            auto nums = TextHelpers::ParseAllNumbers<int32_t>(line);

            if (line.find(TOGGLE) != std::string::npos)
            {
                steps.emplace_back(Instruction { Type::TOGGLE, { nums[0], nums[1] }, { nums[2] + 1, nums[3] + 1 } });
            }
            else if (line.find(TURN_OFF) != std::string::npos)
            {
                steps.emplace_back(Instruction { Type::TURN_OFF, { nums[0], nums[1] }, { nums[2] + 1, nums[3] + 1 } });
            }
            else if (line.find(TURN_ON) != std::string::npos)
            {
                steps.emplace_back(Instruction { Type::TURN_ON, { nums[0], nums[1] }, { nums[2] + 1, nums[3] + 1 } });
            }
        }
    }

    constexpr size_t GRID_SIZE = 999;
    std::vector<int> grid;
    grid.resize(GRID_SIZE * GRID_SIZE, 0);

    auto foreach_2d = [](auto&& grid, glm::ivec2 start, glm::ivec2 end, int32_t stride, auto&& functor)
    {
        for (int x = start.x; x < end.x; x++)
            for (int y = start.y; y < end.y; y++)
            {
                functor(grid[x + y * stride]);
            }
    };

    auto process_bool_grid = [foreach_2d](std::vector<int>& grid, const Instruction& i)
    {
        switch (i.t)
        {
        case Type::TOGGLE:
            foreach_2d(grid, i.start, i.end, static_cast<int32_t>(GRID_SIZE), [](auto& val)
                { val = !val; });
            break;
        case Type::TURN_OFF:
            foreach_2d(grid, i.start, i.end, static_cast<int32_t>(GRID_SIZE), [](auto& val)
                { val = false; });
            break;
        case Type::TURN_ON:
            foreach_2d(grid, i.start, i.end, static_cast<int32_t>(GRID_SIZE), [](auto& val)
                { val = true; });
            break;
        }
    };

    for (auto& i : steps)
    {
        process_bool_grid(grid, i);
    }

    for (auto& elem : grid)
    {
        answer1 += elem;
        elem = 0;
    }

    auto process_int_grid = [foreach_2d](std::vector<int>& grid, const Instruction& i)
    {
        switch (i.t)
        {
        case Type::TOGGLE:
            foreach_2d(grid, i.start, i.end, static_cast<int32_t>(GRID_SIZE), [](auto& val)
                { val += 2; });
            break;
        case Type::TURN_OFF:
            foreach_2d(grid, i.start, i.end, static_cast<int32_t>(GRID_SIZE), [](auto& val)
                { val -= 1; val = std::max(val, 0); });
            break;
        case Type::TURN_ON:
            foreach_2d(grid, i.start, i.end, static_cast<int32_t>(GRID_SIZE), [](auto& val)
                { val += 1; });
            break;
        }
    };

    for (auto& i : steps)
    {
        process_int_grid(grid, i);
    }

    for (auto& elem : grid)
    {
        answer2 += elem;
        elem = 0;
    }

    return { answer1, answer2 };
}
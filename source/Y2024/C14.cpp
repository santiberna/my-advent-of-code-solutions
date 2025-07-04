#include "Utility/Array2D.hpp"
#include "Utility/Log.hpp"
#include "glm/fwd.hpp"
#include <common.hpp>
#include <Utility/TextHelpers.hpp>
#include <Utility/Math.hpp>
#include <thread>
#include <unordered_set>
#include <vector>
#include <chrono>
#include <array>

REGISTER_CHALLENGE_DISABLED(Y2024_CE, "input/Y2024/C14.txt")
{
    struct Robot
    {
        glm::lvec2 position {};
        glm::lvec2 speed {};
    };

    std::vector<Robot> all_robots {};
    glm::lvec2 room_size = { 101, 103 };

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        for (auto& l : lines)
        {
            auto numbers = TextHelpers::ParseAllNumbers<int64_t>(l);
            all_robots.emplace_back(Robot { { numbers[0], numbers[1] }, { numbers[2], numbers[3] } });
        }
    }

    int64_t step_count = 100;
    std::vector<glm::lvec2> final_positions {};

    for (auto& r : all_robots)
    {
        auto final_pos = r.position + (r.speed * step_count);
        final_pos.x = final_pos.x % room_size.x;
        final_pos.y = final_pos.y % room_size.y;

        if (final_pos.x < 0)
            final_pos.x += room_size.x;
        if (final_pos.y < 0)
            final_pos.y += room_size.y;

        final_positions.emplace_back(final_pos);
    }

    int64_t answer1 {}, answer2 {};

    std::array<int64_t, 4> counts {};
    for (auto& r : final_positions)
    {
        auto middle_x = room_size.x / 2;
        auto middle_y = room_size.y / 2;

        auto grid_pos = r;

        // Log::Debug("{} {}", grid_pos.x, grid_pos.y);

        if (grid_pos.x == middle_x || grid_pos.y == middle_y)
            continue;

        bool first = grid_pos.x > middle_x && grid_pos.y > middle_y;
        bool second = grid_pos.x < middle_x && grid_pos.y > middle_y;
        bool third = grid_pos.x < middle_x && grid_pos.y < middle_x;
        bool fourth = grid_pos.x > middle_x && grid_pos.y < middle_y;

        if (first)
            counts[0]++;
        else if (second)
            counts[1]++;
        else if (third)
            counts[2]++;
        else if (fourth)
            counts[3]++;
    }

    answer1 = 1;
    for (auto v : counts)
        answer1 *= v;

    for (int64_t i = 0; i < 100000000; i++)
    {
        std::unordered_set<glm::ivec2> positions {};

        bool invalid = false;
        for (auto r : all_robots)
        {
            auto final_pos = r.position + (r.speed * i);
            final_pos.x = final_pos.x % room_size.x;
            final_pos.y = final_pos.y % room_size.y;

            if (final_pos.x < 0)
                final_pos.x += room_size.x;
            if (final_pos.y < 0)
                final_pos.y += room_size.y;

            if (positions.contains(glm::ivec2(final_pos)))
            {
                invalid = true;
                break;
            }

            positions.emplace(glm::ivec2(final_pos));
        }

        if (invalid)
        {
            // Log::Debug("");
            // Log::Debug("");
            // Log::Debug("Render: {} Invalid", i);

            continue;
        }

        answer2 = i;
        // break;
        Array2D<char> grid { (size_t)room_size.x, (size_t)room_size.y, ' ' };
        for (auto p : positions)
        {
            *grid(p.x, p.y) = '#';
        }

        Log::Debug("");
        Log::Debug("");
        Log::Debug("Render: {}", i);
        grid.Print();
        break;
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return { answer1, answer2 };
}
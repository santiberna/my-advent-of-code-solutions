#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>

#include <Utility/math.hpp>
#include <unordered_set>

REGISTER_CHALLENGE(Y2015_C3_HouseDelivery, "input/Y2015/C3.txt")
{
    std::string instructions {};
    if (auto file = TextHelpers::OpenFileReadStream(input))
    {
        instructions = TextHelpers::StreamToString(file.value());
    }

    auto GetMovementVector = [](char c)
    {
        switch (c)
        {
        case '^':
            return glm::ivec2(0, 1);
        case 'v':
            return glm::ivec2(0, -1);
        case '>':
            return glm::ivec2(1, 0);
        case '<':
            return glm::ivec2(-1, 0);
        default:
            return glm::ivec2(0, 0);
        }
    };

    int64_t answer1 = 0, answer2 = 0;

    {
        glm::ivec2 santa_pos { 0, 0 };
        std::unordered_set<glm::ivec2> houses_reached { santa_pos };

        for (auto i : instructions)
        {
            santa_pos += GetMovementVector(i);
            houses_reached.emplace(santa_pos);
        }

        answer1 = static_cast<int64_t>(houses_reached.size());
    }

    {
        glm::ivec2 santa_pos { 0, 0 };
        glm::ivec2 robo_santa_pos = santa_pos;

        std::unordered_set<glm::ivec2> houses_reached { santa_pos };
        bool use_robo = false;

        for (auto i : instructions)
        {
            auto movement = GetMovementVector(i);

            if (use_robo)
            {
                robo_santa_pos += movement;
                houses_reached.emplace(robo_santa_pos);
            }
            else
            {
                santa_pos += movement;
                houses_reached.emplace(santa_pos);
            }

            use_robo = !use_robo;
        }

        answer2 = static_cast<int64_t>(houses_reached.size());
    }

    return { answer1, answer2 };
}
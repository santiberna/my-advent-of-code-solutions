#include "glm/ext/vector_int2.hpp"
#include <glm/vec2.hpp>
#include <common.hpp>
#include <Utility/TextHelpers.hpp>
#include <Utility/Math.hpp>
#include <unordered_set>
#include <vector>

REGISTER_CHALLENGE(Y2016_C1_Stairs, "input/Y2016/C1.txt")
{
    glm::ivec2 pos { 0, 0 };
    glm::ivec2 dir { 0, 1 };

    std::unordered_set<glm::ivec2> visited {};
    bool check_repeat = true;
    int64_t answer1 {}, answer2 {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto string = TextHelpers::StreamToString(stream.value());

        for (size_t i = 0; i < string.size(); i++)
        {
            auto c = string.at(i);
            bool clockwise {};

            switch (c)
            {
            case 'R':
                clockwise = true;
                break;
            case 'L':
                clockwise = false;
                break;
            default:
                continue;
            }

            dir = TurnVec2(dir, clockwise);

            if (auto num = TextHelpers::ParseNumber<int>({ string.begin() + i + 1, string.end() }))
            {
                pos += dir * num->first;

                if (check_repeat && visited.contains(pos))
                {
                    answer2 = pos.x + pos.y;
                    check_repeat = false;
                }

                visited.emplace(pos);
                i += num->second;
            }
        }
    }

    answer1 = pos.x + pos.y;

    return { answer1, answer2 };
}
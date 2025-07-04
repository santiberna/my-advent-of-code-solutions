#include <glm/vec2.hpp>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <utility/math.hpp>
#include <unordered_set>

REGISTER_CHALLENGE_DISABLED(Y2016_C1_Stairs, "input/Y2016/C1.txt")
{
    struct Delta
    {
        glm::ivec2 dir {};
        int scale {};
    };

    std::vector<Delta> deltas {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto string = TextHelpers::StreamToString(stream.value());
        glm::ivec2 dir { 0, 1 };

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
                deltas.emplace_back(dir, num->first);
            }
        }
    }

    int64_t answer1 {}, answer2 {};

    {
        glm::ivec2 accum = {};

        for (auto d : deltas)
        {
            accum += d.dir * d.scale;
        }
        answer1 = accum.x + accum.y;
    }

    std::unordered_set<glm::ivec2> places_traversed;
    glm::ivec2 accum = {};

    for (auto d : deltas)
    {
        for (int i = 0; i < d.scale; i++)
        {
            auto result = places_traversed.emplace(accum);

            if (!result.second)
            {
                answer2 = accum.x + accum.y;
                break;
            }

            accum += d.dir;
        }
    }

    return { answer1, answer2 };
}
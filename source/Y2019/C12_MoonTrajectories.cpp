#include <iostream>
#include <numeric>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <utility/math.hpp>
#include <vector>

using IVec3 = math::Vec3<int64_t>;

struct Moon
{
    IVec3 position {};
    IVec3 velocity {};
};

void step(std::vector<Moon>& moons)
{
    for (auto& m1 : moons)
    {
        IVec3 gravity {};

        for (auto& m2 : moons)
        {
            if (&m1 == &m2)
                continue;

            auto m1_to_m2 = m2.position - m1.position;

            gravity.x += math::sign(m1_to_m2.x);
            gravity.y += math::sign(m1_to_m2.y);
            gravity.z += math::sign(m1_to_m2.z);
        }

        m1.velocity += gravity;
    }

    for (auto& m1 : moons)
    {
        m1.position += m1.velocity;
    }
}

REGISTER_CHALLENGE(Y2019_CC, "input/Y2019/C12.txt")
{
    std::vector<Moon> initial {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        for (auto& line : lines)
        {
            auto nums = TextHelpers::ParseAllNumbers<int64_t>(line);
            Moon moon {};
            moon.position = { nums.at(0), nums.at(1), nums.at(2) };
            initial.emplace_back(moon);
        }
    }

    int64_t answer1 {}, answer2 {};

    {
        auto moons = initial;
        for (size_t i = 0; i < 1000; ++i)
        {
            step(moons);
        }

        for (auto& m1 : moons)
        {
            auto kin = math::manhattan_distance(m1.velocity);
            auto pot = math::manhattan_distance(m1.position);

            answer1 += kin * pot;
        }
    }

    {
        auto moons = initial;

        std::optional<int64_t> x_loop {};
        std::optional<int64_t> y_loop {};
        std::optional<int64_t> z_loop {};

        int64_t step_count = 0;
        while (!x_loop || !y_loop || !z_loop)
        {
            step_count++;
            step(moons);

            if (!x_loop.has_value())
            {
                bool found = true;
                for (size_t i = 0; i < moons.size(); i++)
                {
                    auto& m1 = moons[i];
                    auto& m2 = initial[i];

                    if (m1.position.x != m2.position.x || m1.velocity.x != m2.velocity.x)
                    {
                        found = false;
                    }
                }

                if (found)
                {
                    x_loop = step_count;
                }
            }

            if (!y_loop.has_value())
            {
                bool found = true;
                for (size_t i = 0; i < moons.size(); i++)
                {
                    auto& m1 = moons[i];
                    auto& m2 = initial[i];

                    if (m1.position.y != m2.position.y || m1.velocity.y != m2.velocity.y)
                    {
                        found = false;
                    }
                }

                if (found)
                {
                    y_loop = step_count;
                }
            }

            if (!z_loop.has_value())
            {
                bool found = true;
                for (size_t i = 0; i < moons.size(); i++)
                {
                    auto& m1 = moons[i];
                    auto& m2 = initial[i];

                    if (m1.position.z != m2.position.z || m1.velocity.z != m2.velocity.z)
                    {
                        found = false;
                    }
                }

                if (found)
                {
                    z_loop = step_count;
                }
            }
        }

        auto lcm1 = std::lcm(*x_loop, *y_loop);
        auto lcm2 = std::lcm(lcm1, *z_loop);

        answer2 = lcm2;
    }
    return { answer1, answer2 };
}

// 3220854
// 3220854
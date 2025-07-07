#include <cstdint>
#include <numeric>
#include <optional>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <utility/array_2d.hpp>
#include <utility>
#include <vector>
#include <utility/math.hpp>
#include <unordered_set>

using IVec2 = math::Vec2<int64_t>;
using DVec2 = math::Vec2<double>;

std::optional<IVec2> shoot_asteroid(
    const std::unordered_set<IVec2>& asteroids,
    const DVec2& float_station,
    const DVec2& direction)
{
    double distance = DBL_MAX;
    std::optional<IVec2> laser_target {};

    for (auto& asteroid : asteroids)
    {
        auto float_asteroid = DVec2(asteroid);
        auto line = float_asteroid - float_station;

        auto normalized = normalize(line);
        auto dot = dot_product(normalized, direction);

        if (math::doubleEq(dot, 1.0))
        {
            auto l = length(line);

            if (l < distance)
            {
                distance = l;
                laser_target = asteroid;
            }
        }
    }

    return laser_target;
}

DVec2 rotate_laser(
    const std::unordered_set<IVec2>& asteroids,
    const DVec2& float_station,
    const DVec2& direction)
{
    double lowest_cos = 0.0f;
    std::optional<DVec2> next_asteroid {};

    for (auto& asteroid : asteroids)
    {
        auto float_asteroid = DVec2(asteroid);
        auto line = float_asteroid - float_station;

        auto cross = cross_product(direction, line);

        if (!math::doubleEq(cross, 0.0) && cross > 0.0)
        {
            auto normalized = normalize(line);
            auto dot = dot_product(normalized, direction);

            if (!math::doubleEq(dot, 0.0) && dot > lowest_cos)
            {
                lowest_cos = dot;
                next_asteroid = float_asteroid;
            }
        }
    }

    if (next_asteroid)
    {
        auto line = next_asteroid.value() - float_station;
        return normalize(line);
    }
    else
    {
        return math::rotate(direction, false);
    }
}

REGISTER_CHALLENGE(Y2019_CA, "input/Y2019/C10.txt")
{
    Array2D<char> grid {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        grid = TextHelpers::StreamToGrid(stream.value());
    }

    int64_t answer1 {}, answer2 {};
    IVec2 chosen_station {};

    {
        std::vector<IVec2> asteroids {};

        for (auto it = grid.begin(); it != grid.end(); ++it)
        {
            if (*it != '#')
                continue;

            asteroids.emplace_back(it.getPosition().first, it.getPosition().second);
        }

        size_t highest = 0;
        for (auto& a1 : asteroids)
        {
            size_t sight = 0;

            for (auto& a2 : asteroids)
            {
                if (&a1 == &a2)
                    continue;

                auto line = a2 - a1;
                auto gcd = std::gcd(line.x, line.y);

                if (gcd == 1)
                {
                    sight++;
                }
                else
                {
                    auto dir = line / gcd;
                    bool obstructed = false;

                    for (int i = 1; i < gcd; i++)
                    {
                        auto check = a1 + dir * i;

                        if (grid.at(check.x, check.y) != '.')
                        {
                            obstructed = true;
                            break;
                        }
                    }

                    if (!obstructed)
                    {
                        sight++;
                    }
                }
            }

            if (sight > highest)
            {
                chosen_station = a1;
                highest = sight;
            }
        }

        answer1 = highest;
    }

    {
        auto float_station = DVec2(chosen_station);
        std::unordered_set<IVec2> asteroids {};

        grid.at(chosen_station.x, chosen_station.y) = 'X';

        std::vector<IVec2> destroyed {};

        for (auto it = grid.begin(); it != grid.end(); ++it)
        {
            if (*it != '#')
                continue;

            auto [x, y] = it.getPosition();
            asteroids.emplace(IVec2(x, y));
        }

        DVec2 direction = { 0.0f, -1.0f };

        while (!asteroids.empty())
        {
            auto to_destroy = shoot_asteroid(asteroids, float_station, direction);

            if (to_destroy)
            {
                destroyed.emplace_back(to_destroy.value());
                asteroids.erase(to_destroy.value());
            }

            direction = rotate_laser(asteroids, float_station, direction);
        }

        answer2 = destroyed[199].x * 100 + destroyed[199].y;
    }

    return { answer1, answer2 };
}
#include <queue>
#include <unordered_set>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <unordered_map>

struct Planet
{
    std::vector<std::string> orbits {};
};

int64_t depthFirstSearch(const std::unordered_map<std::string, Planet>& planets, const std::string& current)
{
    auto& planet = planets.at(current);

    int64_t accum = 0;

    for (auto& orbit : planet.orbits)
    {
        accum += depthFirstSearch(planets, orbit) + 1;
    }
    return accum;
};

REGISTER_CHALLENGE(Y2019_C6, "input/Y2019/C6.txt")
{
    std::unordered_map<std::string, Planet> all_planets {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        for (auto& line : lines)
        {
            auto separator = line.find(')');

            auto name = line.substr(0, separator);
            auto orbit = line.substr(separator + 1);

            all_planets[name].orbits.emplace_back(orbit);
            all_planets.try_emplace(orbit, Planet {});
        }
    }

    int64_t answer1 = 0;

    for (auto& [name, _] : all_planets)
    {
        answer1 += depthFirstSearch(all_planets, name);
    }

    std::unordered_map<std::string, Planet> bidirectional_orbits {};

    for (auto& [name, planet] : all_planets)
    {
        for (auto& orbit : planet.orbits)
        {
            bidirectional_orbits[name].orbits.emplace_back(orbit);
            bidirectional_orbits[orbit].orbits.emplace_back(name);
        }
    }

    int64_t answer2 = 0;

    // breadth first traversal
    {
        std::string start = "YOU";
        std::string target = "SAN";

        struct Step
        {
            std::string name {};
            int64_t cost {};
        };

        std::queue<Step> next;
        next.emplace(start, 0);

        std::unordered_set<std::string> visited {};

        while (!next.empty())
        {
            auto step = next.front();
            next.pop();

            if (step.name == target)
            {
                answer2 = step.cost - 2;
                break;
            }

            visited.emplace(step.name);

            for (auto& orbit : bidirectional_orbits.at(step.name).orbits)
            {
                if (!visited.contains(orbit))
                    next.emplace(orbit, step.cost + 1);
            }
        }
    }

    return { answer1, answer2 };
}

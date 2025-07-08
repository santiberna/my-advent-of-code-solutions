#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <utility/math.hpp>
#include <vector>

struct Component
{
    std::string name;
    int64_t count;
};

struct Equation
{
    Component output {};
    std::vector<Component> ingredients {};
};

using EquationSet = std::unordered_map<std::string, Equation>;
using CountSet = std::unordered_map<std::string, int64_t>;

int64_t craft_fuel(const EquationSet& equations, int64_t fuel_count)
{
    CountSet inventory {};
    CountSet requirements {};

    std::queue<Component> to_create {};
    to_create.push(Component { "FUEL", fuel_count });

    while (!to_create.empty())
    {
        auto next = to_create.front();
        to_create.pop();

        if (next.name == "ORE")
        {
            inventory["ORE"] += next.count;
            continue;
        }

        auto& current_count = inventory[next.name];

        if (current_count >= next.count)
        {
            current_count -= next.count;
            continue;
        }

        next.count -= current_count;
        current_count = 0;

        auto& formula = equations.at(next.name);
        auto iterations = math::divUp(next.count, formula.output.count);

        for (auto& ingredient : formula.ingredients)
        {
            to_create.push(Component { ingredient.name, ingredient.count * iterations });
        }

        auto leftover = iterations * formula.output.count - next.count;
        inventory[next.name] += leftover;
    }

    return inventory.at("ORE");
}

REGISTER_CHALLENGE(Y2019_CE, "input/Y2019/C14.txt")
{
    EquationSet equations {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        // 3 A, 4 B => 1 AB
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        for (auto& line : lines)
        {
            line.erase(std::remove(line.begin(), line.end(), ','), line.end());

            Equation new_eq {};
            auto words = TextHelpers::IntoWords(line);

            for (size_t i = 0; i < words.size() - 3; i += 2)
            {
                auto& count = words.at(i);
                auto& elem = words.at(i + 1);

                auto num = TextHelpers::ParseNumber<int64_t>(count).value().first;
                new_eq.ingredients.emplace_back(Component { elem, num });
            }

            auto& count = words.at(words.size() - 2);
            auto& elem = words.at(words.size() - 1);
            auto num = TextHelpers::ParseNumber<int64_t>(count).value().first;

            new_eq.output = Component { elem, num };

            equations.emplace(new_eq.output.name, new_eq);
        }
    }

    int64_t answer1 {}, answer2 {};

    { // Part 1
        answer1 = craft_fuel(equations, 1);
    }

    { // Part 2
        constexpr int64_t TARGET = 1'000'000'000'000;
        int64_t range_min = math::divUp(TARGET, answer1);
        int64_t range_max = range_min * 2;

        while (range_max - range_min > 1)
        {
            int64_t mid = (range_min + range_max) / 2;
            int64_t value = craft_fuel(equations, mid);

            if (value > TARGET)
            {
                range_max = mid - 1;
            }
            else if (value <= TARGET)
            {
                range_min = mid;
            }
        }

        answer2 = range_max;
    }

    return { answer1, answer2 };
}

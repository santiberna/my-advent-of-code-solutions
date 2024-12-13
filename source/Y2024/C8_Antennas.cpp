#include "Utility/Log.hpp"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include <glm/glm.hpp>
#include <common.hpp>
#include <Utility/TextHelpers.hpp>
#include <unordered_map>
#include <Utility/Math.hpp>
#include <unordered_map>
#include <vector>

REGISTER_CHALLENGE(Y2024_C8, "input/Y2024/C8.txt")
{
    size_t width {}, height {};
    std::vector<std::string> grid {};
    std::unordered_map<char, std::vector<glm::vec2>> antennas {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());
        width = lines.front().size();
        height = lines.size();

        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                auto c = lines[j][i];
                if (c == '.')
                    continue;

                antennas[c].emplace_back(glm::vec2 { i, j });
            }
            grid.emplace_back(std::string(width, '.'));
        }
    }

    int64_t answer1 {}, answer2 {};

    auto calc_distance = [](glm::vec2 a, glm::vec2 b)
    {
        return glm::abs(a.x - b.x) + glm::abs(a.y - b.y);
    };

    auto calc_line = [](glm::vec2 a, glm::vec2 b, glm::vec2 c)
    {
        auto actual = (b - a);
        auto test = (b - c);

        return (actual.y / actual.x) == (test.y / test.x);
    };

    std::vector<std::string> grid2 = grid;

    for (const auto& antenna_type : antennas)
    {
        auto& vector = antenna_type.second;

        for (int a = 0; a < vector.size(); a++)
            for (int b = a + 1; b < vector.size(); b++)
            {
                auto antenna_a = vector[a];
                auto antenna_b = vector[b];

                for (int j = 0; j < height; j++)
                    for (int i = 0; i < width; i++)
                    {
                        glm::vec2 t = { i, j };

                        bool in_line = calc_line(antenna_a, antenna_b, t);
                        bool twice_radius1 = calc_distance(antenna_a, t) == 2 * calc_distance(antenna_b, t);
                        bool twice_radius2 = calc_distance(antenna_b, t) == 2 * calc_distance(antenna_a, t);

                        if (in_line && (twice_radius1 || twice_radius2))
                        {
                            if (grid[j][i] == '.')
                            {
                                grid[j][i] = '#';
                            }
                        }

                        if (in_line || t == antenna_a || t == antenna_b)
                        {
                            grid2[j][i] = '#';
                        }
                    }
            }
    }

    for (size_t j = 0; j < height; j++)
    {
        for (size_t i = 0; i < width; i++)
        {
            if (grid[j][i] == '#')
            {
                answer1++;
            }
            if (grid2[j][i] == '#')
            {
                answer2++;
            }
        }
    }

    return { answer1, answer2 };
}
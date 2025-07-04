#include <cassert>
#include <cstdint>
#include <glm/glm.hpp>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <optional>
#include <vector>
#include <utility/math.hpp>

// 10000000000000

struct Game
{
    glm::lvec2 button_a {};
    glm::lvec2 button_b {};

    glm::lvec2 position {};

    std::vector<glm::lvec2> winning_combos {};
    std::vector<glm::lvec2> winning_combos2 {};
};

std::vector<glm::lvec2> CalculateSolutions(const Game& game, int64_t boundsA, int64_t boundsB)
{
    std::vector<glm::lvec2> out {};
    for (int64_t a = 0; a < boundsA; a++)
    {
        for (int64_t b = 0; b < boundsB; b++)
        {
            auto aVal = game.button_a * a;
            auto bVal = game.button_b * b;

            if (aVal + bVal == game.position)
            {
                out.emplace_back(a, b);
            }
        }
    }

    return out;
}

std::vector<glm::lvec2> CalculateSolutions2(const Game& game)
{
    std::vector<glm::lvec2> out {};

    int64_t a1 = game.button_a.x;
    int64_t a2 = game.button_a.y;

    int64_t b1 = game.button_b.x;
    int64_t b2 = game.button_b.y;

    int64_t c1 = game.position.x;
    int64_t c2 = game.position.y;

    int64_t det = (a1 * b2 - b1 * a2);
    assert(det != 0);

    if ((b2 * c1 - b1 * c2) % det == 0)
    {
        int64_t x = (b2 * c1 - b1 * c2) / det;

        assert((c2 - a2 * x) % b2 == 0);
        int64_t y = (c2 - a2 * x) / b2;

        out.emplace_back(x, y);
    }

    return out;
}

std::optional<int64_t> GetLeastTokens(const std::vector<glm::lvec2>& game)
{

    if (game.empty())
        return std::nullopt;

    assert(game.size() == 1);

    int64_t min = LONG_LONG_MAX;

    for (auto& win : game)
    {
        int64_t tokens = win.x * 3 + win.y * 1;
        min = glm::min(min, tokens);
    }

    return min;
}

REGISTER_CHALLENGE_DISABLED(Y2024_CD, "input/Y2024/C13.txt")
{
    std::vector<Game> games {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        for (size_t i = 0; i < lines.size(); i += 4)
        {
            const auto& line1 = lines[i];
            const auto& line2 = lines[i + 1];
            const auto& line3 = lines[i + 2];

            auto buttonA = TextHelpers::ParseAllNumbers<int64_t>(line1);
            auto buttonB = TextHelpers::ParseAllNumbers<int64_t>(line2);
            auto position = TextHelpers::ParseAllNumbers<int64_t>(line3);

            games.push_back(Game {
                { buttonA[0], buttonA[1] },
                { buttonB[0], buttonB[1] },
                { position[0], position[1] },
                {} });
        }
    }

    int64_t answer1 {}, answer2 {};

    for (auto& game : games)
    {
        int64_t boundACap = 100;
        int64_t boundBCap = 100;

        game.winning_combos = CalculateSolutions2(game);

        auto copy = game;
        copy.position.x += 10'000'000'000'000;
        copy.position.y += 10'000'000'000'000;

        game.winning_combos2 = CalculateSolutions2(copy);

        // auto test = CalculateSolutions2(game);

        // for (size_t i = 0; i < game.winning_combos.size(); i++)
        // {
        //     assert(test[i] == game.winning_combos[i]);
        // }
    }

    for (const auto& game : games)
    {
        if (auto v = GetLeastTokens(game.winning_combos))
            answer1 += v.value();

        if (auto v = GetLeastTokens(game.winning_combos2))
            answer2 += v.value();
    }

    return { answer1, answer2 };
}
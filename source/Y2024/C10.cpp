#include <glm/glm.hpp>
#include <common.hpp>
#include <Utility/TextHelpers.hpp>
#include <Utility/Array2D.hpp>

void EvaluatePathRecursive(int64_t& out, glm::ivec2 position, Array2D<int>& grid, bool overwrite)
{
    auto current_val = *grid(position.x, position.y);

    if (current_val == 9)
    {
        out++;

        if (overwrite)
            *grid(position.x, position.y) = -1;

        return;
    }

    glm::ivec2 available_dirs[] = {
        { 1, 0 },
        { -1, 0 },
        { 0, -1 },
        { 0, 1 }
    };

    for (auto dir : available_dirs)
    {
        auto new_pos = position + dir;
        if (auto* ptr = grid(new_pos.x, new_pos.y))
        {
            if (*ptr == current_val + 1)
                EvaluatePathRecursive(out, new_pos, grid, overwrite);
        }
    }
}

REGISTER_CHALLENGE_DISABLED(Y2024_CA, "input/Y2024/C10.txt")
{
    Array2D<int> grid {};

    if (auto file = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(file.value());

        auto width = lines.front().size();
        auto height = lines.size();

        grid = Array2D<int>(width, height);

        for (size_t j = 0; j < height; j++)
        {
            for (size_t i = 0; i < width; i++)
            {
                *grid(i, j) = lines[j][i] - '0';
            }
        }
    }

    int64_t answer1 {}, answer2 {};

    for (size_t j = 0; j < grid.GetHeight(); j++)
    {
        for (size_t i = 0; i < grid.GetWidth(); i++)
        {
            if (*grid(i, j) == 0)
            {
                int64_t score = 0;

                auto grid_copy = grid;
                EvaluatePathRecursive(score, { i, j }, grid_copy, true);

                answer1 += score;

                int64_t distinct_paths = 0;
                EvaluatePathRecursive(distinct_paths, { i, j }, grid, false);

                answer2 += distinct_paths;
            }
        }
    }

    return { answer1, answer2 };
}
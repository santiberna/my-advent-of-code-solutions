#include <glm/glm.hpp>
#include <common.hpp>
#include <Utility/TextHelpers.hpp>
#include <Utility/Array2D.hpp>
#include <Utility/TextHelpers.hpp>
#include <Utility/Log.hpp>

REGISTER_CHALLENGE(Y2024_C4_MatchXMAS, "input/Y2024/C4.txt")
{
    int64_t answer1 {};
    int64_t answer2 {};

    std::string MATCH = "XMAS";
    size_t MATCH_LENGTH = MATCH.size();

    Array2D<char> grid {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        size_t width = lines.front().size();
        size_t height = lines.size();

        grid = Array2D<char>(width, height);
        for (size_t j = 0; j < height; j++)
            for (size_t i = 0; i < width; i++)
            {
                *grid(i, j) = lines[j][i];
            }
    }

    auto TraverseGrid = [](
                            const Array2D<char>& grid,
                            const glm::ivec2& start,
                            const glm::ivec2& direction,
                            size_t length) -> std::optional<std::string>
    {
        std::string out {};
        glm::ivec2 current = start;

        for (size_t i = 0; i < length; i++)
        {
            size_t x = static_cast<size_t>(current.x);
            size_t y = static_cast<size_t>(current.y);

            if (auto* cell = grid(x, y))
            {
                out.push_back(*cell);
                current += direction;
            }
            else
            {
                return std::nullopt;
            }
        }

        return out;
    };

    glm::ivec2 directions[] = {
        { -1, -1 },
        { -1, 0 },
        { -1, 1 },
        { 0, -1 },
        { 0, 1 },
        { 1, -1 },
        { 1, 0 },
        { 1, 1 },
    };

    // XMAS matching

    for (size_t j = 0; j < grid.GetHeight(); j++)
    {
        for (size_t i = 0; i < grid.GetWidth(); i++)
        {
            auto c = *grid(i, j);
            if (c == MATCH.front())
            {
                for (size_t d = 0; d < 8; d++)
                {
                    auto dir = directions[d];
                    if (auto str = TraverseGrid(grid, { i, j }, dir, MATCH_LENGTH))
                    {
                        if (str.value() == MATCH)
                            answer1++;
                    }
                }
            }
        }
    }

    // MAS in X matching
    for (size_t j = 1; j < grid.GetHeight() - 1; j++)
    {
        for (size_t i = 1; i < grid.GetWidth() - 1; i++)
        {
            if (*grid(i, j) != 'A')
                continue;

            auto subsection = grid.CopySlice<3, 3>(i - 1, j - 1).value();
            std::array<char, 4> relevant {
                subsection[0], subsection[2], subsection[6], subsection[8]
            };

            bool valid = true;
            for (auto c : relevant)
            {
                if (c != 'M' && c != 'S')
                    valid = false;
            }

            if (!valid)
            {

                continue;
            }
            bool cross = relevant[0] != relevant[3] && relevant[1] != relevant[2];
            if (cross)
            {
                answer2++;
            }
        }
    }

    return { answer1, answer2 };
}
#include "Utility/Log.hpp"
#include "glm/fwd.hpp"
#include <Utility/Math.hpp>
#include <array>
#include <cassert>
#include <glm/glm.hpp>
#include <common.hpp>
#include <Utility/TextHelpers.hpp>
#include <Utility/Array2D.hpp>
#include <ostream>
#include <queue>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::vector<glm::ivec2> FloodFill(Array2D<char>& grid, glm::ivec2 start, char fill)
{
    std::vector<glm::ivec2> out {};

    std::array<glm::ivec2, 4> dirs {
        glm::ivec2 { -1, 0 },
        glm::ivec2 { 1, 0 },
        glm::ivec2 { 0, 1 },
        glm::ivec2 { 0, -1 },
    };

    if (auto s = grid(start.x, start.y))
    {
        char match = *s;

        std::queue<glm::ivec2> positions_to_fill;
        positions_to_fill.push(start);
        *grid(start.x, start.y) = fill;

        while (positions_to_fill.size())
        {
            auto next = positions_to_fill.front();
            positions_to_fill.pop();

            out.emplace_back(next);

            for (auto dir : dirs)
            {
                auto pos = next + dir;

                if (auto p = grid(pos.x, pos.y))
                {
                    if (*p == match)
                    {
                        positions_to_fill.emplace(pos);
                        *grid(pos.x, pos.y) = fill;
                    }
                }
            }
        }
    }

    return out;
}

void PrintArray(const Array2D<char>& grid)
{
    for (size_t j = 0; j < grid.GetHeight(); j++)
    {
        for (size_t i = 0; i < grid.GetWidth(); i++)
        {
            std::cout << *grid(i, j);
        }
        std::endl(std::cout);
    }
}

int64_t IsPerimeter(const Array2D<char>& grid, char match, glm::ivec2 pos, bool diagonals = false)
{
    int64_t out {};

    std::array<glm::ivec2, 4> dirs {
        glm::ivec2 { -1, 0 },
        glm::ivec2 { 1, 0 },
        glm::ivec2 { 0, 1 },
        glm::ivec2 { 0, -1 },
    };

    std::array<glm::ivec2, 4> diag {
        glm::ivec2 { -1, -1 },
        glm::ivec2 { 1, 1 },
        glm::ivec2 { -1, 1 },
        glm::ivec2 { 1, -1 },
    };

    for (auto dir : dirs)
    {
        auto p = pos + dir;
        auto ptr = grid(p.x, p.y);

        if (ptr == nullptr)
        {
            out++;
        }
        else if (*ptr != match)
        {
            out++;
        }
    }

    if (diagonals)
        for (auto dir : diag)
        {
            auto p = pos + dir;
            auto ptr = grid(p.x, p.y);

            if (ptr == nullptr)
            {
                out++;
            }
            else if (*ptr != match)
            {
                out++;
            }
        }

    return out;
}

int64_t EvalRegion(const Array2D<char>& grid, const std::vector<glm::ivec2>& spots)
{
    int64_t perimeter {};

    for (auto pos : spots)
    {
        auto match = *grid(pos.x, pos.y);
        perimeter += IsPerimeter(grid, match, pos);
    }

    return perimeter * spots.size();
}

struct HorizontalRange
{
    int start {};
    int end {};
};

std::unordered_map<int, std::vector<HorizontalRange>> GenRanges(const Array2D<char>& grid, const std::vector<glm::ivec2>& spots)
{
    std::unordered_map<int, std::vector<HorizontalRange>> out {};
    std::unordered_set<glm::ivec2> positions { spots.begin(), spots.end() };

    for (int j = 0; j < (int)grid.GetHeight(); j++)
    {
        HorizontalRange current_range {};

        for (int i = -1; i < (int)grid.GetWidth(); i++)
        {

            bool current_match = positions.contains({ i, j });
            bool next_match = positions.contains({ i + 1, j });

            if (!current_match && next_match)
            {
                current_range.start = i + 1;
            }
            else if (current_match && !next_match)
            {
                current_range.end = i;
                out[j].emplace_back(current_range);
            }
        }
    }

    std::unordered_map<int, std::vector<HorizontalRange>> out_filter {};

    for (auto& [k, v] : out)
    {
        if (v.size())
            out_filter.emplace(k, v);
    }

    return out_filter;
}

int64_t EvalRegionDiscount(const Array2D<char>& grid, const std::vector<glm::ivec2>& spots)
{
    auto match = *grid(spots.front().x, spots.front().y);
    int64_t horizontal_sides = 0;

    // Bool means a fence going right
    std::unordered_map<glm::ivec2, bool> fences {};

    for (auto s : spots)
    {
        auto left = s - glm::ivec2 { 1, 0 };
        auto right = s + glm::ivec2 { 1, 0 };

        if (grid.GetOr(left.x, left.y, '.') != match)
        {
            fences.emplace(left, false);
        }

        if (grid.GetOr(right.x, right.y, '.') != match)
        {
            fences.emplace(s, true);
        }
    }

    while (fences.size())
    {
        horizontal_sides++;
        auto [fence, side] = *fences.begin();

        std::vector<glm::ivec2> places_checked {};
        places_checked.push_back(fence);

        auto up_it = fences.find(fence + glm::ivec2 { 0, -1 });

        while (up_it != fences.end())
        {
            if (up_it->second != side)
                break;

            places_checked.push_back(up_it->first);
            up_it = fences.find(up_it->first + glm::ivec2 { 0, -1 });
        }

        auto down_it = fences.find(fence + glm::ivec2 { 0, 1 });

        while (down_it != fences.end())
        {
            if (down_it->second != side)
                break;

            places_checked.push_back(down_it->first);
            down_it = fences.find(down_it->first + glm::ivec2 { 0, 1 });
        }

        for (auto g : places_checked)
        {
            fences.erase(g);
        }
    }

    Log::Debug("{}: {}", match, horizontal_sides * 2);
    return horizontal_sides * 2 * spots.size();
}

REGISTER_CHALLENGE(Y2024_CC, "input/Y2024/C12.txt")
{
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

    int64_t answer1 {}, answer2 {};
    auto copy = grid;
    for (size_t j = 0; j < grid.GetHeight(); j++)
    {
        for (size_t i = 0; i < grid.GetWidth(); i++)
        {
            auto pos = glm::ivec2 { i, j };

            if (*copy(pos.x, pos.y) == '.')
                continue;

            auto region = FloodFill(copy, pos, '.');
            answer1 += EvalRegion(grid, region);
            answer2 += EvalRegionDiscount(grid, region);
        }
    }

    return { answer1, answer2 };
}
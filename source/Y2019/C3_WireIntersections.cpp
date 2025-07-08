#include <algorithm>
#include <istream>
#include <optional>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <utility/math.hpp>
#include <sstream>
#include <vector>

using IVec2 = math::Vec2<int>;

struct Line
{
    IVec2 start {};
    IVec2 end {};
};

std::optional<IVec2> findIntersect(const Line& a, const Line& b)
{
    // Detect vertical and horizontal segments
    bool aVertical = (a.start.x == a.end.x);
    bool bVertical = (b.start.x == b.end.x);

    // Only handle one vertical and one horizontal segment
    if (aVertical == bVertical)
    {
        return std::nullopt;
    }

    // Assign vertical/horizontal segments
    const Line& vert = aVertical ? a : b;
    const Line& hori = aVertical ? b : a;

    int vx = vert.start.x;
    int hy = hori.start.y;

    int vyMin = std::min(vert.start.y, vert.end.y);
    int vyMax = std::max(vert.start.y, vert.end.y);
    int hxMin = std::min(hori.start.x, hori.end.x);
    int hxMax = std::max(hori.start.x, hori.end.x);

    if (vx >= hxMin && vx <= hxMax && hy >= vyMin && hy <= vyMax)
    {
        return IVec2 { vx, hy };
    }

    return std::nullopt; // No intersection
}

std::vector<IVec2> parse_line(std::istream& line)
{
    IVec2 start {};
    std::vector<IVec2> ret { start };

    auto lines = TextHelpers::SplitStreamIntoLines(line, ',');

    for (auto& line : lines)
    {
        auto first = line.front();
        auto num = TextHelpers::ParseNumber<int>({ line.begin() + 1, line.end() }).value().first;
        IVec2 vec {};

        switch (first)
        {
        case 'R':
            vec.x = +num;
            break;
        case 'L':
            vec.x = -num;
            break;
        case 'U':
            vec.y = -num;
            break;
        case 'D':
            vec.y = +num;
            break;
        default:
            throw;
        }

        start += vec;
        ret.emplace_back(start);
    }

    return ret;
}

REGISTER_CHALLENGE(Y2019_C3, "input/Y2019/C3.txt")
{
    std::vector<IVec2> line1 {};
    std::vector<IVec2> line2 {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        std::stringstream first {};
        first << lines.at(0);

        line1 = parse_line(first);

        std::stringstream second {};
        second << lines.at(1);

        line2 = parse_line(second);
    }

    std::vector<int64_t> intersection_distances {}, intersection_lengths {};

    size_t line1_progress {};

    for (size_t i = 0; i < line1.size() - 1; ++i)
    {
        Line segment1 = { line1.at(i), line1.at(i + 1) };

        size_t line2_progress {};

        for (size_t j = 0; j < line2.size() - 1; ++j)
        {
            Line segment2 = { line2.at(j), line2.at(j + 1) };

            if (auto intersect = findIntersect(segment1, segment2))
            {
                auto dist1 = math::manhattan_distance(*intersect - segment1.start);
                auto dist2 = math::manhattan_distance(*intersect - segment2.start);

                intersection_distances.emplace_back(math::manhattan_distance(*intersect));
                intersection_lengths.emplace_back(dist1 + dist2 + line1_progress + line2_progress);
            }

            line2_progress += math::manhattan_distance(segment2.start - segment2.end);
        }

        line1_progress += math::manhattan_distance(segment1.start - segment1.end);
    }

    intersection_distances.erase(intersection_distances.begin());
    intersection_lengths.erase(intersection_lengths.begin());

    int64_t answer1 = *std::min_element(intersection_distances.begin(), intersection_distances.end());
    int64_t answer2 = *std::min_element(intersection_lengths.begin(), intersection_lengths.end());

    return { answer1, answer2 };
}
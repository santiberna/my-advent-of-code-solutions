#include <cassert>
#include <limits>
#include <unordered_map>
#include <utility/array_2d.hpp>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <utility/math.hpp>
#include <queue>
#include <array>
#include <vector>

using IVec2 = math::Vec2<int64_t>;

constexpr IVec2 DIRECTIONS[] = {
    IVec2 { 0, -1 }, IVec2 { 1, 0 }, IVec2 { 0, 1 }, IVec2 { -1, 0 }
};

using KeyFlags = uint64_t;
using Locations = std::unordered_map<char, IVec2>;

IVec2 locationFromNode(const Locations& locations, uint32_t node)
{
    if (node == 0)
    {
        return locations.at('@');
    }
    else
    {
        return locations.at(node - 1 + 'a');
    }
}

KeyFlags AddKey(KeyFlags prev, char k)
{
    assert(k - 'a' >= 0 && k - 'a' < 64);
    return prev | 1 << (k - 'a');
}

bool CheckKey(KeyFlags prev, char k)
{
    assert(k - 'a' >= 0 && k - 'a' < 64);
    return prev & (1 << (k - 'a'));
}

struct Part1State
{
    uint64_t node = 0;
    KeyFlags keys = 0;
    int64_t cost = 0;

    bool operator<(const Part1State& o) const { return cost < o.cost; }
};

std::vector<int64_t> findPaths(const Array2D<char>& map, const Locations& key_locations, uint32_t start_node, KeyFlags keys)
{
    std::vector<int64_t> out(key_locations.size(), 0);

    Array2D<int64_t> visited { map.getWidth(), map.getHeight(), -1 };
    auto start = locationFromNode(key_locations, start_node);

    visited.fill(-1);
    visited.at(start.x, start.y) = 0;

    std::queue<IVec2> to_check;
    to_check.push(start);

    while (!to_check.empty())
    {
        auto next = to_check.front();
        to_check.pop();

        auto cost = visited.at(next.x, next.y) + 1;

        for (auto d : DIRECTIONS)
        {
            auto new_pos = next + d;
            auto grid_val = map.at(new_pos.x, new_pos.y);

            bool is_empty = grid_val == '.';
            bool not_visited = visited.at(new_pos.x, new_pos.y) == -1;

            bool is_door = key_locations.contains(std::tolower(grid_val));
            bool is_open_door = false;

            bool is_key = key_locations.contains(grid_val);
            bool is_collected_key = false;

            if (is_door)
            {
                is_open_door = CheckKey(keys, std::tolower(grid_val));
            }

            if (is_key)
            {
                is_collected_key = CheckKey(keys, grid_val);
            }

            if (key_locations.contains(grid_val) && !CheckKey(keys, grid_val))
            {
                out.at(grid_val - 'a' + 1) = cost;
            }

            if ((is_empty || is_open_door || is_collected_key) && not_visited)
            {
                visited.at(new_pos.x, new_pos.y) = cost;
                to_check.push(new_pos);
            }
        }
    }

    return out;
}

int64_t part1(const Array2D<char>& original_grid)
{
    auto grid = original_grid;

    KeyFlags KEY_COMPLETE = 0;
    std::unordered_map<char, IVec2> locations {};

    for (auto it = grid.begin(); it != grid.end(); ++it)
    {
        auto pos = IVec2(it.getPosition().first, it.getPosition().second);

        if (*it == '@')
        {
            locations[*it] = pos;
            *it = '.';
        }

        if (*it >= 'a' && *it <= 'z')
        {
            KEY_COMPLETE = AddKey(KEY_COMPLETE, *it);
            locations[*it] = pos;
        }
    }

    std::unordered_map<IVec2, std::vector<int64_t>> cached_graphs {};
    std::unordered_map<IVec2, int64_t> prev_states {};

    std::priority_queue<Part1State> states {};
    states.push(Part1State {});

    int64_t best_cost = std::numeric_limits<int64_t>::max();

    while (!states.empty())
    {
        auto step = states.top();
        states.pop();

        if (step.cost >= best_cost)
        {
            continue;
        }

        if (auto it = prev_states.find(IVec2(step.node, step.keys)); it != prev_states.end())
        {
            if (step.cost >= it->second)
            {
                continue;
            }
            else
            {
                it->second = step.cost;
            }
        }
        else
        {
            prev_states.emplace(IVec2(step.node, step.keys), step.cost);
        }

        if (step.keys == KEY_COMPLETE)
        {
            best_cost = step.cost;
            continue;
        }

        std::vector<int64_t>* graph = nullptr;

        if (auto it = cached_graphs.find(IVec2(step.node, step.keys)); it != cached_graphs.end())
        {
            graph = &it->second;
        }
        else
        {
            auto result = findPaths(grid, locations, step.node, step.keys);
            auto [iter, res] = cached_graphs.emplace(IVec2(step.node, step.keys), result);
            graph = &iter->second;
        }

        for (size_t i = 0; i < graph->size(); i++)
        {
            auto node = i;
            auto cost = graph->at(node);

            if (cost == 0)
            {
                continue;
            }

            Part1State new_state {};

            new_state.cost = step.cost + cost;
            new_state.node = node;
            new_state.keys = AddKey(step.keys, node - 1 + 'a');

            states.emplace(new_state);
        }
    }

    return best_cost;
}

struct Part2State
{
    std::array<uint64_t, 4> positions {};
    KeyFlags keys {};
    int64_t cost = 0;

    bool operator<(const Part1State& o) const { return cost < o.cost; }
};

int64_t part2(const Array2D<char>& original_grid)
{
    auto grid = original_grid;

    KeyFlags KEY_COMPLETE = 0;
    std::unordered_map<char, IVec2> locations {};

    IVec2 start {};

    for (auto it = grid.begin(); it != grid.end(); ++it)
    {
        auto pos = IVec2(it.getPosition().first, it.getPosition().second);

        if (*it == '@')
        {
            start = pos;
        }

        if (*it >= 'a' && *it <= 'z')
        {
            KEY_COMPLETE = AddKey(KEY_COMPLETE, *it);
            locations[*it] = pos;
        }
    }

    grid.at(start.x - 1, start.y - 1) = '0';
    grid.at(start.x, start.y - 1) = '#';
    grid.at(start.x + 1, start.y - 1) = '1';
    grid.at(start.x - 1, start.y) = '#';
    grid.at(start.x, start.y) = '#';
    grid.at(start.x + 1, start.y) = '#';
    grid.at(start.x - 1, start.y + 1) = '2';
    grid.at(start.x, start.y + 1) = '#';
    grid.at(start.x + 1, start.y + 1) = '3';

    std::cout << grid << std::endl;
    return 0;
}

REGISTER_CHALLENGE(Y2019_CI, "input/Y2019/C18.txt")
{
    Array2D<char> grid {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        grid = TextHelpers::StreamToGrid(stream.value());
    }

    return { part1(grid), part2(grid) };
}

#include "utility/array_2d.hpp"
#include "utility/timer.hpp"
#include <cctype>
#include <climits>
#include <float.h>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <utility/math.hpp>
#include <vector>
#include <stack>
#include <queue>

using IVec2 = math::Vec2<int64_t>;

constexpr IVec2 DIRECTIONS[] = {
    IVec2 { 0, -1 }, IVec2 { 1, 0 }, IVec2 { 0, 1 }, IVec2 { -1, 0 }
};

struct State
{
    IVec2 position {};
    size_t current_cost {};
    std::unordered_set<char> collected_keys {};
    std::vector<char> path {};

    bool operator<(const State& o) const
    {
        return current_cost < o.current_cost;
    }

    bool operator==(const State& o) const
    {
        bool same_keys = collected_keys == o.collected_keys;
        bool same_pos = position == o.position;

        return same_keys && same_pos;
    }
};

namespace std
{
template <>
struct hash<State>
{
    std::size_t operator()(const State& s) const
    {
        std::size_t seed = 0;

        // Hash the position
        math::hash_combine(seed, std::hash<IVec2>()(s.position));

        // Hash the set of collected keys (order-insensitive)
        for (char key : s.collected_keys)
        {
            math::hash_combine(seed, std::hash<char>()(key));
        }

        return seed;
    }
};
}

REGISTER_CHALLENGE(Y2019_CI, "input/Y2019/C18.txt")
{
    Array2D<char> grid {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        grid = TextHelpers::StreamToGrid(stream.value());
    }

    int64_t answer1 {}, answer2 {};
    IVec2 start {};

    struct KeyDoorPair
    {
        IVec2 key {};
        IVec2 door {};
    };

    std::unordered_map<char, KeyDoorPair> keys {};

    for (auto it = grid.begin(); it != grid.end(); ++it)
    {
        auto pos = IVec2(it.getPosition().first, it.getPosition().second);

        if (*it == '@')
        {
            start = pos;
            *it = '.';
        }

        if (*it >= 'a' && *it <= 'z')
        {
            keys[*it].key = pos;
        }

        if (*it >= 'A' && *it <= 'Z')
        {
            keys[std::tolower(*it)].door = pos;
        }
    }

    { // Part 1

        State best_path { {}, INT_MAX, {}, {} };

        std::priority_queue<State> branches {};
        std::vector<State> evaluated {};

        evaluated.reserve(40 * 1024);

        branches.emplace(State { start, 0, {}, {} });

        Array2D<int64_t> visited { grid.getWidth(), grid.getHeight(), -1 };

        while (!branches.empty())
        {
            auto branch = branches.top();
            branches.pop();

            if (branch.current_cost >= best_path.current_cost)
            {
                continue;
            }

            // if (auto it = evaluated.find(branch); it != evaluated.end())
            // {
            //     if (it->current_cost <= branch.current_cost)
            //     {
            //         break;
            //     }
            // }

            bool skip = false;

            for (auto& eval : evaluated)
            {
                if (eval == branch)
                {
                    if (eval.current_cost <= branch.current_cost)
                    {
                        skip = true;
                    }
                    else if (eval.current_cost > branch.current_cost)
                    {
                        eval = branch;
                    }
                }
            }

            if (skip)
                continue;

            evaluated.emplace_back(branch);

            if (branch.collected_keys.size() == keys.size())
            {
                // std::cout << branch.path << std::endl;
                // std::cout << branch.current_cost << std::endl;

                best_path = branch;
            }

            // Find all close keys
            std::vector<std::pair<char, size_t>> found_keys {};

            {
                visited.fill(-1);
                visited.at(branch.position.x, branch.position.y) = 0;

                std::queue<IVec2> to_check;
                to_check.push(branch.position);

                while (!to_check.empty())
                {
                    auto next = to_check.front();
                    to_check.pop();

                    auto cost = visited.at(next.x, next.y) + 1;

                    for (auto d : DIRECTIONS)
                    {
                        auto new_pos = next + d;
                        auto grid_val = grid.at(new_pos.x, new_pos.y);

                        bool is_empty = grid_val == '.';
                        bool not_visited = visited.at(new_pos.x, new_pos.y) == -1;

                        bool is_door = keys.contains(std::tolower(grid_val));
                        bool is_open_door = false;

                        bool is_key = keys.contains(grid_val);
                        bool is_collected_key = false;

                        if (is_door)
                        {
                            is_open_door = branch.collected_keys.contains(std::tolower(grid_val));
                        }

                        if (is_key)
                        {
                            is_collected_key = branch.collected_keys.contains(grid_val);
                        }

                        if (keys.contains(grid_val) && !branch.collected_keys.contains(grid_val))
                        {
                            found_keys.emplace_back(grid.at(new_pos.x, new_pos.y), cost);
                        }

                        if ((is_empty || is_open_door || is_collected_key) && not_visited)
                        {
                            visited.at(new_pos.x, new_pos.y) = cost;
                            to_check.push(new_pos);
                        }
                    }
                }
            }

            for (auto [key, cost] : found_keys)
            {
                State new_branch {};

                new_branch.collected_keys = branch.collected_keys;
                new_branch.collected_keys.emplace(key);

                new_branch.position = keys.at(key).key;
                new_branch.current_cost = branch.current_cost + cost;

                new_branch.path = branch.path;
                new_branch.path.emplace_back(key);

                branches.emplace(new_branch);
            }
        }

        answer1 = best_path.current_cost;
    }

    return { answer1, answer2 };
}

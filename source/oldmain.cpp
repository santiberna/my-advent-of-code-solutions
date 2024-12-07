#include <array>
#include <bit>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "util/combinations.hpp"
#include "util/graph.hpp"
#include "util/json.hpp"
#include "util/log.hpp"
#include "util/math_helpers.hpp"
#include "util/md5_hash.hpp"
#include "util/text_helpers.hpp"
#include "util/thread_pool.hpp"

#define SDL_DISABLE_ANALYZE_MACROS
#include <SDL3/SDL.h>

namespace Y2015
{

void C7()
{
    std::vector<std::string> lines;
    if (auto input = OpenFileReadStream("input/2015/7.txt"))
    {
        lines = SplitStreamIntoLines(input.value());
    }

    enum class Operation
    {
        NONE,
        AND,
        OR,
        NOT,
        RSHIFT,
        LSHIFT
    };
    struct BinaryOp
    {
        std::string first, second;
        Operation op;
    };
    struct UnaryOp
    {
        std::string var;
        Operation op;
    };

    using Term = std::variant<uint16_t, std::string, BinaryOp, UnaryOp>;
    std::unordered_map<std::string, Term> circuit;

    for (auto& str : lines)
    {
        auto tokens = SplitString(str, " ");
        size_t token_count = tokens.size();

        if (token_count == 3) // n -> var
        {
            if (auto num = ParseNumber<uint16_t>(tokens.front()))
            {
                circuit.emplace(tokens.back(), num.value());
            }
            else
            {
                circuit.emplace(tokens.back(), tokens.front());
            }
        }
        else if (token_count == 4) // NOT n -> var
        {
            circuit.emplace(tokens.back(), UnaryOp { tokens.at(1), Operation::NOT });
        }
        else if (token_count == 5)
        {
            BinaryOp op {};
            op.first = tokens.front();
            op.second = tokens.at(2);

            auto& operand = tokens.at(1);
            if (operand == "AND")
            {
                op.op = Operation::AND;
            }
            else if (operand == "OR")
            {
                op.op = Operation::OR;
            }
            else if (operand == "RSHIFT")
            {
                op.op = Operation::RSHIFT;
            }
            else if (operand == "LSHIFT")
            {
                op.op = Operation::LSHIFT;
            }

            circuit.emplace(tokens.back(), op);
        }
    }

    struct VisitorSolver
    {
        const std::unordered_map<std::string, Term> circuit;
        std::unordered_map<std::string, uint16_t> cache;

        uint16_t solve(const std::string& var)
        {
            if (auto it = cache.find(var); it != cache.end())
            {
                return it->second;
            }
            else
            {
                auto& operation = circuit.find(var)->second;
                auto result = std::visit(*this, operation);
                cache.emplace(var, result);

                return result;
            }
        }

        uint16_t operator()(uint16_t v) { return v; };

        uint16_t operator()(const std::string& v) { return solve(v); };

        uint16_t operator()(const UnaryOp& v)
        {
            auto result = solve(v.var);
            return ~(result);
        };

        uint16_t operator()(const BinaryOp& v)
        {
            uint16_t result1 {};
            uint16_t result2 {};

            if (auto n = ParseNumber<uint16_t>(v.first))
            {
                result1 = n.value();
            }
            else
            {
                result1 = solve(v.first);
            }

            if (auto n = ParseNumber<uint16_t>(v.second))
            {
                result2 = n.value();
            }
            else
            {
                const auto& second = (*circuit.find(v.second)).second;
                result2 = solve(v.second);
            }

            switch (v.op)
            {
            case Operation::AND:
            {
                return result1 & result2;
            }
            case Operation::OR:
            {
                return result1 | result2;
            }
            case Operation::LSHIFT:
            {
                return result1 << result2;
            }
            case Operation::RSHIFT:
            {
                return result1 >> result2;
            }
            default:
                return -1;
            }
        };
    };

    VisitorSolver solver { circuit };

    std::string target = "a";
    uint16_t result = solver.solve(target);
    Log("First Run: {}", result);

    // Rewire result -> b

    circuit["b"] = result;
    VisitorSolver solver2 { circuit };

    uint16_t result2 = solver2.solve(target);
    Log("Second Run: {}", result2);

    Log("Challenge Seven Complete");
}

void C8()
{
    std::vector<std::string> lines;

    if (auto in = OpenFileReadStream("input/2015/8.txt"))
    {
        lines = SplitStreamIntoLines(in.value());
    }

    // Decoding sequence

    size_t code_characters {};
    size_t string_characters {};
    for (auto& str : lines)
    {
        code_characters += str.size();
        std::string copy = str;

        // Double slashes
        copy = ReplaceAll(copy, "\\\\", "/");

        // Hexadecimal characters

        size_t it = 0;
        while ((it = copy.find("\\x")) != std::string::npos)
        {
            auto substr = copy.substr(it + 2, 2);
            auto num = ParseHex<int>(substr);

            if (num)
            {
                copy.replace(size_t(it), 4, 1, (char)(num.value()));
            }
        }

        // Escape sequences
        copy = ReplaceAll(copy, "\\\"", "\'");
        copy = copy.substr(1, copy.size() - 2);
        string_characters += copy.size();
    }

    // Encoding sequence

    size_t expanded_characters {};
    for (auto& str : lines)
    {
        std::string copy = str;
        copy = ReplaceAll(copy, "\"", "/\"");
        copy = ReplaceAll(copy, "\\", "\\\\");

        expanded_characters += copy.size() + 2;
    }

    Log("Literal Chars - String Chars = {}", code_characters - string_characters);
    Log("Expanded Chars - Literal Chars = {}",
        expanded_characters - code_characters);
    Log("Challenge Eight Complete");
}

void C9()
{
    std::vector<std::string> lines;
    if (auto in = OpenFileReadStream("input/2015/9.txt"))
    {
        lines = SplitStreamIntoLines(in.value());
    }

    std::unordered_map<std::string, size_t> places;
    size_t count = 0;

    for (auto& str : lines)
    {
        auto tokens = SplitString(str, " ");
        if (tokens.size() >= 3)
        {
            if (auto it = places.find(tokens.front()); it == places.end())
            {
                places.emplace(tokens.front(), count);
                count++;
            }
            if (auto it = places.find(tokens[2]); it == places.end())
            {
                places.emplace(tokens[2], count);
                count++;
            }
        }
    }

    AdjacencyMatrix graph { count };

    for (auto& str : lines)
    {
        auto tokens = SplitString(str, " ");
        if (tokens.size() < 5)
            continue;

        auto& from = tokens[0];
        auto& to = tokens[2];
        auto& distance = tokens[4];

        size_t start = places[from];
        size_t end = places[to];
        float weight = ParseNumber<float>(distance).value();

        graph.AddUndirectedEdge(start, end, weight);
    }

    using Path = std::vector<size_t>;
    using PathArray = std::vector<Path>;

    std::function<void(PathArray & out, const Path& path,
        std::unordered_set<size_t> not_visited)>
        GeneratePaths;
    GeneratePaths = [&](PathArray& out, const Path& path,
                        const std::unordered_set<size_t>& not_visited)
    {
        if (not_visited.empty())
        {
            out.emplace_back(std::move(path));
        }
        for (auto i : not_visited)
        {
            auto copy = path;
            auto n = not_visited;

            copy.push_back(i);
            n.erase(i);

            GeneratePaths(out, copy, n);
        }
    };

    PathArray all_paths {};
    std::unordered_set<size_t> n;
    for (size_t i = 0; i < graph.GetSize(); i++)
        n.insert(i);

    GeneratePaths(all_paths, {}, n);

    std::vector<float> path_distances;

    for (auto& path : all_paths)
    {
        float path_sum = 0;
        for (size_t i = 0; i < path.size() - 1; i++)
        {
            auto from = path[i];
            auto to = path[i + 1];

            path_sum += *graph.At(from, to);
        }

        path_distances.push_back(path_sum);
    }

    auto it = std::min_element(path_distances.begin(), path_distances.end());
    auto it2 = std::max_element(path_distances.begin(), path_distances.end());

    Log("Minimum distance for all Places: {}", *it);
    Log("Maximum distance for all Places: {}", *it2);
    Log("Challenge Nine Complete");
}

void C10()
{
    std::string input = "3113322113";

    auto expand = [](const std::string& in)
    {
        std::vector<std::pair<char, size_t>> sequences;
        for (auto c : in)
        {
            if (sequences.empty())
            {
                sequences.emplace_back(c, 1);
            }
            else if (c != sequences.back().first)
            {
                sequences.emplace_back(c, 1);
            }
            else
            {
                sequences.back().second++;
            }
        }

        // Stringify
        std::string out {};
        for (auto [c, s] : sequences)
        {
            out += char(s) + '0';
            out += c;
        }

        return out;
    };

    for (int i = 0; i < 50; i++)
        input = expand(input);

    Log("Expanded Sequence Size: {}", input.size());
    Log("Challenge Ten complete");
}

void C11()
{
    std::string input = "hxbxwxba";

    std::vector<int> digits(input.size());
    for (int i = 0; i < input.size(); ++i)
    {
        digits.at(i) = input.at(i) - 'a';
    }

    std::function<void(std::vector<int> & digits, int rightmost)> Increment;
    Increment = [&](std::vector<int>& digits, int rightmost)
    {
        if (rightmost == -1)
            return;
        digits.at(rightmost) = (digits.at(rightmost) + 1) % 26;

        if (digits.at(rightmost) == 0)
        {
            Increment(digits, rightmost - 1);
        }
    };

    // Check straight sequences
    auto CheckStraight = [](const std::vector<int>& digits)
    {
        for (int i = 1; i < digits.size() - 1; ++i)
        {
            int left_diff = digits.at(i) - digits.at(i - 1);
            int right_diff = digits.at(i + 1) - digits.at(i);

            if (left_diff == 1 && right_diff == 1)
            {
                return true;
            }
        }
        return false;
    };

    // Check ILO
    auto CheckILO = [](const std::vector<int>& digits)
    {
        for (int i = 0; i < digits.size(); ++i)
        {
            int current = digits.at(i);
            if (current == ('i' - 'a') || current == ('l' - 'a') || current == ('o' - 'a'))
            {
                return false;
            }
        }
        return true;
    };

    // Check Pairs
    auto CheckDoublePair = [](const std::vector<int>& digits)
    {
        int pairs = 0;
        for (int i = 0; i < (int)digits.size() - 1; ++i)
        {
            int current = digits.at(i);
            int next = digits.at(i + 1);

            if (current == next)
            {
                pairs++;
                i++;
                if (pairs == 2)
                    return true;
            }
        }
        return false;
    };

    auto GenPassword = [](const std::vector<int>& digits)
    {
        std::string newPassword;
        for (auto digit : digits)
        {
            newPassword += char(digit) + 'a';
        }
        return newPassword;
    };

    bool validPassword = false;
    while (!validPassword)
    {
        Increment(digits, (int)(digits.size() - 1));
        validPassword = CheckILO(digits) && CheckDoublePair(digits) && CheckStraight(digits);
    }

    Log("Found First Valid Password: {}", GenPassword(digits));

    validPassword = false;
    while (!validPassword)
    {
        Increment(digits, (int)(digits.size() - 1));
        validPassword = CheckILO(digits) && CheckDoublePair(digits) && CheckStraight(digits);
    }

    Log("Found Second Valid Password: {}", GenPassword(digits));
    Log("Challenge Eleven complete");
}

void C12()
{
    auto input = OpenFileReadStream("input/2015/12.txt");
    if (!input)
    {
        Log("Failed To Read JSON input file");
        return;
    }

    class JSONSumVisitor
    {
    public:
        double ProcessValue(const JSONValue& v)
        {
            return std::visit(*this, v.data);
        }

        double operator()(std::monostate) { return 0.0; };
        double operator()(bool) { return 0.0; };
        double operator()(double v) { return v; };
        double operator()(const std::string& s) { return 0.0; };

        double operator()(const std::vector<JSONValue>& object)
        {
            double sum = 0.0f;
            for (auto& val : object)
            {
                sum += ProcessValue(val);
            }
            return sum;
        }
        double
        operator()(const std::unordered_map<std::string, JSONValue>& object)
        {
            double sum = 0.0f;
            for (auto& [key, val] : object)
            {
                if (val.IsString())
                {
                    if (std::get<std::string>(val.data) == "red")
                        return 0.0;
                }

                sum += ProcessValue(val);
            }

            if (sum != sum)
                sum = 0.0;
            return sum;
        };

        bool invalidate = false;
    };

    std::stringstream err;
    JSONParser parser(&err);

    auto result = parser.ParseValue(input.value());
    if (!result)
    {
        Log("Failed to parse JSON: {}", err.str());
        return;
    }

    Log("Sum of all JSON elements (excluding \"red\"): {}",
        JSONSumVisitor().ProcessValue(result.value()));
    Log("Challenge Twelve Complete");
}

void C13()
{
    std::vector<std::string> lines;

    if (auto input = OpenFileReadStream("input/2015/13.txt"))
    {
        lines = SplitStreamIntoLines(input.value());
    }

    std::unordered_set<std::string> unique_names;
    std::vector<std::tuple<std::string, int, std::string>> data;

    for (auto& line : lines)
    {
        line.pop_back(); // Pop trailing '.'
        auto tokens = SplitString(line, " ");

        if (tokens.size() < 2)
            continue;

        unique_names.emplace(tokens.front());
        unique_names.emplace(tokens.back());

        int sign = -1;
        size_t number_index = -1;

        if (auto i = line.find("gain"); i != std::string::npos)
        {
            number_index = i;
            sign = 1;
        }
        else if (auto i = line.find("lose"); i != std::string::npos)
        {
            number_index = i;
            sign = -1;
        }

        std::string n_str = line.substr(number_index + 5);
        if (auto num = ParseNumber<int>(n_str))
        {
            data.emplace_back(tokens.front(), num.value() * sign, tokens.back());
        }
    }

    std::vector<std::string> names(unique_names.begin(), unique_names.end());

    auto FindIndex = [](std::vector<std::string>& vec, const std::string& look)
    {
        auto it = std::find_if(vec.begin(), vec.end(),
            [&](const auto& e)
            { return e == look; });
        return it - vec.begin();
    };

    AdjacencyMatrix graph { names.size() };
    for (auto& [s, d, e] : data)
    {
        auto start = FindIndex(names, s);
        auto end = FindIndex(names, e);

        graph.AddDirectedEdge(start, end, static_cast<float>(d));
    }

    // graph.Print();

    using Path = std::vector<size_t>;
    using PathArray = std::vector<Path>;

    std::function<void(PathArray & out, const Path& path,
        std::unordered_set<size_t> not_visited)>
        GeneratePaths;
    GeneratePaths = [&](PathArray& out, const Path& path,
                        const std::unordered_set<size_t>& not_visited)
    {
        if (not_visited.empty())
        {
            out.emplace_back(std::move(path));
        }
        for (auto i : not_visited)
        {
            auto copy = path;
            auto n = not_visited;

            copy.push_back(i);
            n.erase(i);

            GeneratePaths(out, copy, n);
        }
    };

    {

        PathArray all_paths {};
        std::unordered_set<size_t> n;
        for (size_t i = 1; i < graph.GetSize(); i++)
            n.insert(i);

        GeneratePaths(all_paths, { 0 }, n);

        std::vector<float> path_distances;

        for (auto& path : all_paths)
        {
            float path_sum = 0;
            for (size_t i = 0; i < path.size(); i++)
            {
                auto from = path[i];
                auto to = path[(i + 1) % path.size()];

                float val = (*graph.At(from, to) + *graph.At(to, from));
                path_sum += val;
            }

            path_distances.push_back(path_sum);
        }

        auto it = std::max_element(path_distances.begin(), path_distances.end());
        Log("Happiest Room setup provides: {} happiness", *it);
    }

    {
        PathArray all_paths {};

        // Adding myself
        graph.AddNode();
        // graph.Print();

        std::unordered_set<size_t> m;
        for (size_t i = 1; i < graph.GetSize(); i++)
            m.insert(i);

        all_paths.clear();
        GeneratePaths(all_paths, { 0 }, m);

        std::vector<float> path_distances;
        for (auto& path : all_paths)
        {
            float path_sum = 0;
            for (size_t i = 0; i < path.size(); i++)
            {
                auto from = path[i];
                auto to = path[(i + 1) % path.size()];

                float val = (*graph.At(from, to) + *graph.At(to, from));
                path_sum += val;
            }

            path_distances.push_back(path_sum);
        }

        auto it2 = std::max_element(path_distances.begin(), path_distances.end());
        Log("Happiest Room setup provides (including myself): {} happiness", *it2);
    }
    Log("Challenge Thirteen Complete");
}

void C14()
{
    struct Runner
    {
        std::string name;
        int speed, sprint_time, rest_time;

        int CalculateDistance(int seconds) const
        {
            int cycle_time = rest_time + sprint_time;
            int cycles_complete = seconds / cycle_time;
            int distance_per_cycle = speed * sprint_time;
            int remaining_seconds = seconds % cycle_time;
            int remaining_distance = std::min(remaining_seconds, sprint_time) * speed;

            return cycles_complete * distance_per_cycle + remaining_distance;
        }
    };

    std::vector<Runner> all_runners;

    if (auto input = OpenFileReadStream("input/2015/14.txt"))
    {
        auto lines = SplitStreamIntoLines(input.value());
        for (auto& line : lines)
        {
            auto numbers = ParseAllNumbers<int>(line);
            if (numbers.size() >= 3)
            {
                all_runners.emplace_back(std::string(), numbers[0], numbers[1],
                    numbers[2]);
            }

            auto words = SplitString(line, " ");
            if (words.size())
                all_runners.back().name = words.front();
        }
    }

    int race_time = 2503;

    std::sort(all_runners.begin(), all_runners.end(),
        [&](const auto& lhs, const auto& rhs)
        {
            return lhs.CalculateDistance(race_time) > rhs.CalculateDistance(race_time);
        });

    Log("Race 1 - Fastest Racer: {} with a distance of {}",
        all_runners.front().name,
        all_runners.front().CalculateDistance(race_time));

    std::vector<int> distances_travelled;
    distances_travelled.resize(all_runners.size());
    std::vector<int> score_accumulated;
    score_accumulated.resize(all_runners.size());

    for (int t = 1; t <= race_time; t++)
    {
        for (size_t r = 0; r < all_runners.size(); r++)
        {
            auto& runner = all_runners[r];
            distances_travelled[r] = runner.CalculateDistance(t);
        }

        auto it = std::max_element(distances_travelled.begin(),
            distances_travelled.end());
        for (size_t r = 0; r < all_runners.size(); r++)
        {
            if (distances_travelled[r] == *it)
                score_accumulated[r]++;
        }
    }

    auto it = std::max_element(score_accumulated.begin(), score_accumulated.end());
    size_t w = it - score_accumulated.begin();

    Log("Race 2 - Pointiest Racer: {} with a score of {}", all_runners[w].name,
        *it);
    Log("Challenge Fourteen Complete");
}

void C15()
{
    using InputMatrix = TMat<int, 5, 4>;
    InputMatrix input;

    if (auto in = OpenFileReadStream("input/2015/15.txt"))
    {
        auto lines = SplitStreamIntoLines(in.value());

        using Transposed = TMat<int, 4, 5>;
        Transposed::ArrayType data {};

        size_t r = 0;
        for (auto line : lines)
        {
            auto numbers = ParseAllNumbers<int>(line);
            for (size_t c = 0; c < input.GetRows(); c++)
            {
                data[r * input.GetRows() + c] = numbers[c];
            }
            r++;
        }

        Transposed t = { data };
        input = t.Transpose();
        input.Print(std::cout);
    }

    int result = 0;

    for (int x = 0; x < 100; x++)
        for (int y = 0; y < 100; y++)
            for (int z = 0; z < 100; z++)
            {
                int w = 100 - x - y - z;
                if (w <= 0)
                    continue;

                using TestMat = TMat<int, 4, 1>;
                auto mul = input.Mul(TestMat({ x, y, z, w }));

                int val = 1;
                for (size_t i = 0; i < 4; i++)
                {
                    val *= std::clamp(mul.At(i, 0), 0, std::numeric_limits<int>::max());
                }

                if (mul.At(4, 0) == 500)
                {
                    result = std::max(result, val);
                }
            }

    Log("Highest Score Cookie: {}", result);
    Log("Challenge Fifteen Complete!");
}

void C16()
{
    std::vector<std::string> keywords {
        "children", "cats", "samoyeds", "pomeranians", "akitas",
        "vizslas", "goldfish", "trees", "cars", "perfumes"
    };

    constexpr int CAT_NR = 1;
    constexpr int TREE_NR = 7;

    constexpr int POMERANIANS_NR = 3;
    constexpr int GOLDFISH_NR = 6;

    constexpr int WILDCARD = -1;
    using Sue = std::array<int, 10>;

    std::vector<Sue> all_sues;

    if (auto input = OpenFileReadStream("input/2015/16.txt"))
    {
        auto lines = SplitStreamIntoLines(input.value());

        for (auto line : lines)
        {
            Sue sue {};
            for (size_t i = 0; i < keywords.size(); i++)
            {
                auto& word = keywords[i];
                auto it = line.find(word);

                if (it != std::string::npos)
                {
                    auto substring = line.substr(it + word.size() + 2);

                    sue[i] = ParseNumber<int>(substring).value();
                }
                else
                {
                    sue[i] = WILDCARD;
                }
            }

            all_sues.push_back(sue);
        }
    }

    Sue pattern = { 3, 7, 2, 3, 0, 0, 5, 3, 2, 1 };

    std::vector<std::pair<Sue, size_t>> matches;

    for (size_t j = 0; j < all_sues.size(); j++)
    {
        auto& sue = all_sues[j];

        bool match = true;
        for (size_t i = 0; i < 10; i++)
        {
            auto& s = sue[i];
            auto& m = pattern[i];

            if (s == WILDCARD)
                continue;

            if (i == CAT_NR || i == TREE_NR) // Lower equal
            {
                if (s <= m)
                    match = false;
            }
            else if (i == POMERANIANS_NR || i == GOLDFISH_NR) // Greater equal
            {
                if (s >= m)
                    match = false;
            }
            else // Unequal
            {
                if (s != m)
                    match = false;
            }
        }

        if (match)
        {
            matches.emplace_back(sue, j + 1);
        }
    }

    Log("Aunt Sue {} gave you the gift!", matches.front().second);
    Log("Challenge Sixteen Complete!");
}

void C17()
{
    int target = 150;
    std::vector<int> buckets;

    if (auto in = OpenFileReadStream("input/2015/17.txt"))
    {
        auto lines = SplitStreamIntoLines(in.value());
        for (auto l : lines)
        {
            buckets.push_back(ParseNumber<int>(l).value());
        }
    }

    std::unordered_map<size_t, size_t> counts;

    std::function<void(const std::vector<int>& buckets, int* count, int index,
        int current, int used)>
        FindCombinations;
    FindCombinations = [&](const std::vector<int>& buckets, int* count, int index,
                           int current, int used) mutable
    {
        if (current == 0)
        {
            (*count)++;
            counts[used]++;

            return;
        }

        for (int i = index; i < (int)buckets.size(); ++i)
        {
            if (current - buckets[i] >= 0)
            {
                FindCombinations(buckets, count, i + 1, current - buckets[i], used + 1);
            }
        }
    };

    int count = 0;
    FindCombinations(buckets, &count, 0, target, 0);

    Log("{} many ways to arrange {} liters", count, target);
    Log("Challenge Seventeen Complete!");
}

void C18()
{
    std::vector<std::string> lines;

    if (auto in = OpenFileReadStream("input/2015/18.txt"))
    {
        lines = SplitStreamIntoLines(in.value());
    }

    size_t width = lines.front().size(), height = lines.size();

    Grid<int> first = { width, height };
    Grid<int> second = { width, height };

    for (size_t j = 0; j < height; j++)
        for (size_t i = 0; i < width; i++)
        {
            auto c = lines[j][i];
            if (c == '#')
                first.Set(i, j, 1);
            else
                first.Set(i, j, 0);
        }

    // first.Print();

    auto CheckNeighbours = [](const Grid<int>& in, int x, int y)
    {
        int ret = 0;
        for (int j = y - 1; j <= y + 1; j++)
            for (int i = x - 1; i <= x + 1; i++)
            {
                if (j < 0 || i < 0)
                    continue;
                if (j == y && i == x)
                    continue;

                if (auto val = in.At(i, j))
                {
                    if (val.value() == 1)
                        ret++;
                }
            }
        return ret;
    };

    Grid<int>* selected[] = { &first, &second };
    size_t current = 0;

    for (size_t t = 0; t < 100; t++)
    {
        size_t next = (current + 1) % 2;

        auto& in = *selected[current];
        auto& out = *selected[next];

        // Set corners
        in.Set(0, 0, 1);
        in.Set(width - 1, 0, 1);
        in.Set(0, height - 1, 1);
        in.Set(width - 1, height - 1, 1);

        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++)
            {
                int val = in.At(x, y).value();
                int neighbours = CheckNeighbours(in, x, y);

                int output_val = 2;

                if (val == 1)
                {
                    if (neighbours == 2 || neighbours == 3)
                    {
                        output_val = 1;
                    }
                    else
                    {
                        output_val = 0;
                    }
                }
                else
                {
                    if (neighbours == 3)
                    {
                        output_val = 1;
                    }
                    else
                    {
                        output_val = 0;
                    }
                }

                out.Set(x, y, output_val);
            }

        // Set corners
        out.Set(0, 0, 1);
        out.Set(width - 1, 0, 1);
        out.Set(0, height - 1, 1);
        out.Set(width - 1, height - 1, 1);

        // Log("");
        // out.Print();
        current = next;
    }

    size_t on = 0;
    for (size_t y = 0; y < height; y++)
        for (size_t x = 0; x < width; x++)
        {
            if (first.At(x, y).value() == 1)
                on++;
        }

    Log("Lights on at the end of the Animation: {}", on);
    Log("Challenge Eighteen Complete");
}

void C19()
{
    std::string molecule {};
    std::unordered_map<std::string, std::string> rules {};

    if (auto in = OpenFileReadStream("input/2015/19.txt"))
    {
        auto lines = SplitStreamIntoLines(in.value());

        for (auto l : lines)
        {
            bool is_rule = l.find("=>") != std::string::npos;

            if (is_rule)
            {
                auto words = SplitString(l);
                rules.emplace(words.back(), words.front());
            }
            else
            {
                molecule = l;
            }
        }
    }

    std::string accum;
    std::vector<std::string> elements;

    for (auto it = molecule.begin(); it != molecule.end(); it++)
    {
        accum.push_back(*it);

        auto next = it + 1;
        if (next == molecule.end() || std::isupper(*next) || *next == 'e')
        {
            elements.emplace_back(accum);
            accum.clear();
        }
    }

    size_t rn_ar_count = 0;
    size_t y_count = 0;

    for (const auto& str : elements)
    {
        if (str == "Ar" || str == "Rn")
        {
            rn_ar_count++;
        }
        else if (str == "Y")
        {
            y_count++;
        }
    }

    size_t result = elements.size() - rn_ar_count - y_count * 2 - 1;

    Log("Minimum replacements needed: {}", result);
    Log("Challenge Nineteen Complete");
}

void C20()
{
    int puzzle_input = 34000000;

    /*std::vector<int> precalculated_primes{};

    if (auto stream = OpenFileReadStream("input/primes1.txt"))
    {
            auto& ss = stream.value();

            std::string number{};
            while (std::getline(ss, number, ' '))
            {
                    if (auto n = ParseNumber<int>(number))
                    {
                            if (n.value() > puzzle_input) break;
                            precalculated_primes.push_back(n.value());
                    }
            }
    }*/

    /*auto factorize = [](std::span<int> prime_table, int number) ->
    std::vector<int>
            {
                    std::vector<int> out{};
                    int prime_index = 0;

                    while (number > 1)
                    {
                            int current_prime = prime_table[prime_index++];
                            int multiplicity = 0;

                            while (number % current_prime == 0)
                            {
                                    multiplicity++;
                                    number /= current_prime;
                            }

                            out.emplace_back(multiplicity);
                    }
                    return out;
            };

    auto div_sum = [](std::span<int> prime_table, std::span<int> multiplicity) ->
    int
            {
                    int out = 1;

                    for (size_t i = 0; i < multiplicity.size(); i++)
                    {
                            int prime = prime_table[i];
                            int mult = multiplicity[i];

                            int upper = PowerOfInt(prime, mult + 1) - 1;
                            int lower = prime - 1;
                            out *= (upper / lower);
                    }

                    return out;
            };

    int i = 781199;
    while (++i)
    {
            auto factors = factorize(precalculated_primes, i);
            int divisors = div_sum(precalculated_primes, factors);
            int presents = divisors;

            int cutoff = i / 50;
            int subtract = 0;

            for (int i = 0; i < cutoff; i++)
            {
                    if (divisors % (i + 1) == 0)
                            subtract += (i + 1);
            }

            int result = presents - subtract;

            if (result * 11 > puzzle_input)
            {
                    Log("House Number found: {}", i);
                    Log("Total presents delivered: {}", result * 11);

                    break;
            }

            if (i % 10000 == 0) Log("At {}", i);
    }*/

    auto sum1 = [](int n)
    {
        int sum = 0;
        int d = (int)sqrt((double)n) + 1;

        for (int i = 1; i <= d; i++)
        {
            if (n % i == 0)
            {
                sum += i;
                sum += n / i;
            }
        }
        return sum;
    };

    auto sum2 = [](int n)
    {
        int sum = 0;
        int d = (int)std::sqrt((double)n) + 1;

        for (int i = 1; i <= d; i++)
        {
            if (n % i == 0)
            {
                if (i <= 50)
                {
                    sum += n / i;
                }
                if (n / i <= 50)
                {
                    sum += i;
                }
            }
        }
        return sum;
    };

    int part1 = 0;
    while (sum1(part1) * 10 < puzzle_input)
        part1++;

    int part2 = 0;
    while (sum2(part2) * 11 < puzzle_input)
        part2++;

    Log("Answers: {}, {}", part1, part2);
    Log("Challenge Twenty Complete");
}

void C21()
{
    std::vector<std::pair<int, int>> weapon_shop {
        { 8, 4 },
        { 10, 5 },
        { 25, 6 },
        { 40, 7 },
        { 74, 8 },
    };

    std::vector<std::pair<int, int>> armor_shop {
        { 0, 0 },
        { 13, 1 },
        { 31, 2 },
        { 53, 3 },
        { 75, 4 },
        { 102, 5 },
    };

    std::vector<std::tuple<int, int, int>> ring_shop {
        { 0, 0, 0 }, { 0, 0, 0 }, { 20, 1, 0 }, { 40, 2, 0 },
        { 80, 3, 0 }, { 25, 0, 1 }, { 50, 0, 2 }, { 100, 0, 3 }
    };

    struct Stats
    {
        int hitpoints = 0;
        int armor = 0;
        int damage = 0;
    };

    auto fight = [](const Stats& a, const Stats& b) -> bool
    {
        int a_att = std::max(a.damage - b.armor, 1);
        int b_att = std::max(b.damage - a.armor, 1);

        int ha = a.hitpoints;
        int hb = b.hitpoints;

        while (true)
        {
            hb -= a_att;
            if (hb <= 0)
            {
                return true;
            }

            ha -= b_att;
            if (ha <= 0)
            {
                return false;
            }
        }

        /*int a_rounds = (int)std::ceilf((float)a.hitpoints / (float)b_att);
        int b_rounds = (int)std::ceilf((float)b.hitpoints / (float)a_att);

        return a_rounds >= b_rounds;*/
    };

    Stats player = { 100, 0, 0 };
    Stats boss = { 104, 1, 8 };

    std::vector<int> gold_amounts {};

    for (auto& selected_weapon : weapon_shop)
        for (auto& selected_armor : armor_shop)
        {
            for (auto& acc1 : ring_shop)
                for (auto& acc2 : ring_shop)
                {
                    if (&acc1 == &acc2)
                        continue;

                    Stats test = player;
                    int gold_spent = selected_weapon.first + selected_armor.first + std::get<0>(acc1) + std::get<0>(acc2);
                    test.armor = selected_armor.second + std::get<1>(acc1) + std::get<1>(acc2);
                    test.damage = selected_weapon.second + std::get<2>(acc1) + std::get<2>(acc2);

                    if (!fight(test, boss))
                    {
                        Log("Lost battle with: {}/{} - {}", test.damage, test.armor,
                            gold_spent);
                        gold_amounts.push_back(gold_spent);
                    }
                }
        }

    Log("Minimum gold: {}",
        *std::max_element(gold_amounts.begin(), gold_amounts.end()));
    Log("Challenge 21 Complete");
}

void C22()
{
    enum class Choice
    {
        MISSILE, // 53 4
        DRAIN, // 73 2 2
        SHIELD, // 113 R6 7
        POISON, // 173 R6 3
        RECHARGE // 229 R5 101
    };

    enum class State
    {
        RUNNING,
        VICTORY,
        DEFEAT,
        INVALID
    };

    class FightContext
    {
        int current_health = 50;
        int current_mana = 500;
        int total_mana_spent = 0;

        int shield_timer = 0;
        int poison_timer = 0;
        int recharge_timer = 0;

        int boss_health = 51;
        int boss_damage = 9;

    public:
        int GetSpent() const { return total_mana_spent; }

        State Next(Choice c)
        {
            // DOT

            current_health--;
            if (current_health <= 0)
                return State::DEFEAT;

            // Apply effects

            if (shield_timer > 0)
            {
                shield_timer--;
            }

            if (poison_timer > 0)
            {
                boss_health -= 3;
                poison_timer--;

                if (boss_health <= 0)
                {
                    return State::VICTORY;
                }
            }

            if (recharge_timer > 0)
            {
                recharge_timer--;
                current_mana += 101;
            }

            // Use Choice

            switch (c)
            {
            case Choice::MISSILE:
                if (current_mana < 53)
                    return State::INVALID;

                current_mana -= 53;
                total_mana_spent += 53;
                boss_health -= 4;

                if (boss_health <= 0)
                    return State::VICTORY;

                break;

            case Choice::DRAIN:
                if (current_mana < 73)
                    return State::INVALID;
                current_mana -= 73;
                total_mana_spent += 73;

                current_health += 2;
                boss_health += -2;

                if (boss_health <= 0)
                    return State::VICTORY;

                break;

            case Choice::POISON:
                if (current_mana < 173)
                    return State::INVALID;
                if (poison_timer > 0)
                    return State::INVALID;

                current_mana -= 173;
                total_mana_spent += 173;
                poison_timer += 6;

                break;

            case Choice::SHIELD:
                if (current_mana < 113)
                    return State::INVALID;
                if (shield_timer > 0)
                    return State::INVALID;

                current_mana -= 113;
                total_mana_spent += 113;
                shield_timer += 6;

                break;

            case Choice::RECHARGE:
                if (current_mana < 229)
                    return State::INVALID;
                if (recharge_timer > 0)
                    return State::INVALID;

                current_mana -= 229;
                total_mana_spent += 229;
                recharge_timer += 5;

                break;
            }

            // Boss Turn
            int temp_armor = 0;

            if (shield_timer > 0)
            {
                shield_timer--;
                temp_armor = 7;
            }

            if (poison_timer > 0)
            {
                boss_health -= 3;
                poison_timer--;

                if (boss_health <= 0)
                {
                    return State::VICTORY;
                }
            }

            if (recharge_timer > 0)
            {
                recharge_timer--;
                current_mana += 101;
            }

            current_health -= std::max(1, boss_damage - temp_armor);

            if (current_health <= 0)
                return State::DEFEAT;

            return State::RUNNING;
        }
    };

    const FightContext starting {};
    int lowest_mana_spent = std::numeric_limits<int>::max();

    std::queue<FightContext> queue {};
    queue.push(starting);

    auto process_choice = [&queue, &lowest_mana_spent](const FightContext& f,
                              Choice c)
    {
        FightContext n = f;
        State result = n.Next(c);

        switch (result)
        {
        case State::RUNNING:
            queue.push(n);
            break;
        case State::VICTORY:
            lowest_mana_spent = std::min(lowest_mana_spent, n.GetSpent());
            break;
        default:
            break;
        }
    };

    while (queue.size())
    {
        const FightContext this_fight = queue.front();
        queue.pop();

        if (this_fight.GetSpent() >= lowest_mana_spent)
            continue;

        process_choice(this_fight, Choice::MISSILE);
        process_choice(this_fight, Choice::DRAIN);
        process_choice(this_fight, Choice::POISON);
        process_choice(this_fight, Choice::RECHARGE);
        process_choice(this_fight, Choice::SHIELD);
    }

    Log("Lowest Mana to defeat boss: {}", lowest_mana_spent);
    Log("Challenge 22 Complete!");
}

void C23()
{
    struct VirtualMachine
    {
        const size_t INSTRUCTION_SIZE = 3;
        const char OFFSET_START = ',';

        const char* HALF = "hlf";
        const char* TRIPLE = "tpl";
        const char* INC = "inc";
        const char* JMP = "jmp";
        const char* JMP_EVEN = "jie";
        const char* JMP_ONE = "jio";

        uint32_t registers[2] { 0, 0 };
        int32_t instruction_ptr { 0 };

        void run(std::vector<std::string>& program)
        {
            do
            {
                instruction_ptr = read_line(program[instruction_ptr]);
            } while (instruction_ptr < (int32_t)program.size() && instruction_ptr >= 0);
        }

    private:
        uint32_t get_register(const std::string& line)
        {
            return line[1 + INSTRUCTION_SIZE] - 'a';
        }

        int32_t get_number(const std::string& line, size_t off)
        {
            auto view = std::string_view(line).substr(off);
            if (view.starts_with('+'))
            {
                view = view.substr(1);
            }

            return ParseNumber<int32_t>(view).value();
        }

        int32_t read_line(const std::string& line)
        {
            // HALF
            if (size_t i = line.find(HALF); i != std::string::npos)
            {
                uint32_t reg = get_register(line);
                registers[reg] /= 2;
                return instruction_ptr + 1;
            }

            // TRIPLE
            if (size_t i = line.find(TRIPLE); i != std::string::npos)
            {
                uint32_t reg = get_register(line);
                registers[reg] *= 3;
                return instruction_ptr + 1;
            }

            // INC
            if (size_t i = line.find(INC); i != std::string::npos)
            {
                uint32_t reg = get_register(line);
                registers[reg] += 1;
                return instruction_ptr + 1;
            }

            // JMP
            if (size_t i = line.find(JMP); i != std::string::npos)
            {
                int32_t offset = get_number(line, INSTRUCTION_SIZE + 1);
                return instruction_ptr + offset;
            }

            // JIE
            if (size_t i = line.find(JMP_EVEN); i != std::string::npos)
            {
                uint32_t reg = get_register(line);

                if (registers[reg] % 2 == 0)
                {
                    int32_t offset = get_number(line, line.find(OFFSET_START) + 2);
                    return instruction_ptr + offset;
                }

                return instruction_ptr + 1;
            }

            // JIO
            if (size_t i = line.find(JMP_ONE); i != std::string::npos)
            {
                uint32_t reg = get_register(line);

                if (registers[reg] == 1)
                {
                    int32_t offset = get_number(line, line.find(OFFSET_START) + 2);
                    return instruction_ptr + offset;
                }

                return instruction_ptr + 1;
            }

            throw;
        }
    };

    VirtualMachine vm {};
    vm.registers[0] = 1;

    if (auto stream = OpenFileReadStream("input/2015/23.txt"))
    {
        auto lines = SplitStreamIntoLines(stream.value());
        vm.run(lines);
    }

    Log("Register b : {}", vm.registers[1]);
    Log("Challenge 23 Complete!");
}

void C24()
{
    std::vector<int> all_packages;

    if (auto stream = OpenFileReadStream("input/2015/24.txt"))
    {
        auto lines = SplitStreamIntoLines(stream.value());
        for (auto& l : lines)
        {
            all_packages.emplace_back(ParseNumber<int>(l).value());
        }
    }

    int box_sum = std::accumulate(all_packages.begin(), all_packages.end(), 0);
    constexpr int TARGET_SIZE = 6;

    auto res = FindCombinations(all_packages, TARGET_SIZE);
    std::vector<std::array<int, TARGET_SIZE>> possibilities {};

    for (size_t j = 0; j < res.GetHeight(); j++)
    {
        int sum = std::accumulate(&res(0, j), &res(TARGET_SIZE, j), 0);
        if (sum == box_sum / 3)
        {
            std::array<int, TARGET_SIZE> t {};
            std::copy(&res(0, j), &res(TARGET_SIZE, j), t.begin());
            possibilities.emplace_back(t);
        }
    }

    auto check_sum = [](const std::vector<int>& remaining, int target)
    {
        for (size_t i = 1; i < remaining.size(); i++)
        {
            auto check = FindCombinations(remaining, i);
            size_t width = check.GetWidth();

            for (size_t j = 0; j < check.GetHeight(); j++)
            {
                int sum = std::accumulate(&check(0, j), &check(width, j), 0);
                if (sum == target)
                {
                    return true;
                }
            }
        }

        return false;
    };

    std::vector<std::array<int, TARGET_SIZE>> checked {};

    for (const auto& p : possibilities)
    {
        std::vector<int> remaining = all_packages;

        for (auto i : p)
        {
            std::remove_if(remaining.begin(), remaining.end(),
                [=](const auto& l)
                { return l == i; });
            remaining.pop_back();
        }

        if (check_sum(remaining, box_sum / 3))
        {
            checked.emplace_back(p);
        }
    }

    // Log("{}", possibilities);
    Log("Challenge 23 Complete!");
}

} // namespace Y2015

int main(int argc, char* argv[])
{
    std::cout << "Hello World" << std::endl;

    if (argc > 1 && std::string(argv[1]) == "-T")
    {
        return 0;
    }

    // Y2015::C1();
    // Y2015::C2();
    // Y2015::C3();
    // Y2015::C4();
    // Y2015::C5();
    // Y2015::C6();
    // Y2015::C7();
    // Y2015::C8();
    // Y2015::C9();
    // Y2015::C10();

    // Y2015::C11();
    // Y2015::C12();
    // Y2015::C13();
    // Y2015::C14();
    // Y2015::C15();
    // Y2015::C16();
    // Y2015::C17();
    // Y2015::C18();
    // Y2015::C19();
    // Y2015::C20();

    // Y2015::C21();
    // Y2015::C22();
    // Y2015::C23();
    // Y2015::C24();

    return 0;
}
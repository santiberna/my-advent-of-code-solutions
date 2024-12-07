#include <Utility/TextHelpers.hpp>
#include <common.hpp>
#include <variant>

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

REGISTER_CHALLENGE(Y2015_C7_BinaryCircuit, "input/Y2015/C7.txt")
{
    std::unordered_map<std::string, Term> circuit;

    if (auto file = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(file.value());

        for (auto& str : lines)
        {
            auto tokens = TextHelpers::SplitString(str, " ");
            size_t token_count = tokens.size();

            if (token_count == 3) // n -> var
            {
                if (auto num = TextHelpers::ParseNumber<uint16_t>(tokens.front()))
                {
                    circuit.emplace(tokens.back(), num->first);
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

            if (auto n = TextHelpers::ParseNumber<uint16_t>(v.first))
            {
                result1 = n->first;
            }
            else
            {
                result1 = solve(v.first);
            }

            if (auto n = TextHelpers::ParseNumber<uint16_t>(v.second))
            {
                result2 = n->first;
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

    int64_t answer1 {}, answer2 {};
    VisitorSolver solver { circuit };

    std::string target = "a";
    answer1 = solver.solve(target);

    // Rewire result -> b

    circuit["b"] = static_cast<uint16_t>(answer1);
    VisitorSolver solver2 { circuit };

    answer2 = solver2.solve(target);

    return { answer1, answer2 };
}
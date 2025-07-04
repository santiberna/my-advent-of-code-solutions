#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

struct VM_State
{
    int64_t i {};
    int64_t a {};
    int64_t b {};
    int64_t c {};
};

enum OpCode : int64_t
{
    ADV, // Div A by 2^Co -> A
    BXL, // XOR B by N -> B
    BST, // Co % 3 -> B
    JNZ, // A -> I
    BXC, // B XOR C -> B
    OUT, // Co -> OUTPUT
    BDV, // Div A by 2^Co -> B
    CDV, // Div A by 2^Co -> C
};

struct Operation
{
    int64_t code {};
    int64_t operand {};
};

REGISTER_CHALLENGE_DISABLED(Y2024_CH, "input/Y2024/C17.txt")
{
    VM_State state {};
    std::vector<Operation> operations {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto lines = TextHelpers::SplitStreamIntoLines(stream.value());

        state.a = TextHelpers::ParseAllNumbers<int64_t>(lines[0]).front();
        state.b = TextHelpers::ParseAllNumbers<int64_t>(lines[1]).front();
        state.c = TextHelpers::ParseAllNumbers<int64_t>(lines[2]).front();

        auto numbers = TextHelpers::ParseAllNumbers<int64_t>(lines.back());

        for (size_t i = 0; i < numbers.size() - 1; i += 2)
        {
            auto op = numbers[i];
            auto lit = numbers[i + 1];
            operations.emplace_back(op, lit);
        }
    }

    int64_t answer1 {}, answer2 {};

    auto GetComboLiteral = [](const VM_State& s, int64_t operand)
    {
        if (operand == 4)
        {
            return s.a;
        }
        if (operand == 5)
        {
            return s.b;
        }
        if (operand == 6)
        {
            return s.c;
        }
        if (operand == 7)
        {
            throw;
        }
        return operand;
    };

    // enum OpCode : int64_t
    // {
    //     ADV, // Div A by 2^Co -> A
    //     BXL, // XOR B by N -> B
    //     BST, // Co % 3 -> B
    //     JNZ, // A -> I
    //     BXC, // B XOR C -> B
    //     OUT, // Co -> OUTPUT
    //     BDV, // Div A by 2^Co -> B
    //     CDV, // Div A by 2^Co -> C
    // };

    std::vector<int64_t> output {};

    while (state.i < operations.size())
    {
        assert(state.a >= 0);
        assert(state.b >= 0);
        assert(state.c >= 0);

        auto instruction = operations[state.i];
        auto combo = GetComboLiteral(state, instruction.operand);

        switch (instruction.code)
        {
        case OpCode::ADV:
        {
            state.a = state.a / (0b1 << combo);
            state.i++;
            break;
        }
        case OpCode::BDV:
        {
            state.b = state.a / (0b1 << combo);
            state.i++;
            break;
        }
        case OpCode::CDV:
        {
            state.c = state.a / (0b1 << combo);
            state.i++;
            break;
        }
        case OpCode::BXL:
        {
            state.b = state.b ^ instruction.operand;
            state.i++;
            break;
        }
        case OpCode::BXC:
        {
            state.b = state.b ^ state.c;
            state.i++;
            break;
        }
        case OpCode::OUT:
        {
            output.emplace_back(combo % 8);
            state.i++;
            break;
        }
        case OpCode::BST:
        {
            state.b = GetComboLiteral(state, instruction.operand) % 8;
            state.i++;
            break;
        }
        case OpCode::JNZ:
        {
            if (state.a != 0)
            {
                state.i = instruction.operand;
            }
            else
            {
                state.i++;
            }
            break;
        }
        default:
        {
            throw;
        }
        }
    }

    std::stringstream str {};
    for (auto c : output)
        str << c;
    auto string = str.str();

    if (string.size())
        answer1 = std::stoll(string);

    return { answer1, answer2 };
}
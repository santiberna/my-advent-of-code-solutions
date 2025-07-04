#include "utility/Log.hpp"
#include <cstdint>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <vector>

enum OpCode : int64_t
{
    ADD = 1,
    MULTIPLY = 2,
    STOP = 99
};

bool should_continue(const std::vector<int64_t>& mem, size_t ip)
{
    return ip < mem.size() && mem.at(ip) != STOP;
};

int64_t run_program(std::vector<int64_t> memory, int64_t noun, int64_t verb)
{
    memory.at(1) = noun;
    memory.at(2) = verb;

    for (size_t ip = 0; should_continue(memory, ip);)
    {
        auto op_code = memory.at(ip);

        switch (op_code)
        {
        case ADD:
        {
            auto num1 = memory.at(memory.at(ip + 1));
            auto num2 = memory.at(memory.at(ip + 2));
            memory.at(memory.at(ip + 3)) = num1 + num2;

            ip += 4;
            break;
        }
        case MULTIPLY:
        {
            auto num1 = memory.at(memory.at(ip + 1));
            auto num2 = memory.at(memory.at(ip + 2));
            memory.at(memory.at(ip + 3)) = num1 * num2;

            ip += 4;
            break;
        }
        default:
            throw;
        }
    }

    return memory.at(0);
}

REGISTER_CHALLENGE(Y2019_C2, "input/Y2019/C2.txt")
{
    std::vector<int64_t> program { 1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50 };

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto line = TextHelpers::StreamToString(stream.value());
        program = TextHelpers::ParseAllNumbers<int64_t>(line);
    }

    int64_t answer2 = -1;

    {
        for (int64_t noun = 0; noun < 100; noun++)
        {
            for (int64_t verb = 0; verb < 100; verb++)
            {
                auto result = run_program(program, noun, verb);
                if (result == 19690720)
                {
                    answer2 = noun * 100 + verb;
                }
            }
        }
    }

    return { run_program(program, 12, 2), answer2 };
}

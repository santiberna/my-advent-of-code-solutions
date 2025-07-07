#include <utility/int_code.hpp>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>

REGISTER_CHALLENGE(Y2019_C5, "input/Y2019/C5.txt")
{
    ic::Program program {};
    program.state = { 3, 21, 1008, 21, 8, 20, 1005, 20, 22, 107, 8, 21, 20, 1006, 20, 31,
        1106, 0, 36, 98, 0, 0, 1002, 21, 125, 20, 4, 20, 1105, 1, 46, 104,
        999, 1105, 1, 46, 1101, 1000, 1, 20, 4, 20, 1105, 1, 46, 98, 99 };

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto line = TextHelpers::StreamToString(stream.value());
        program.state = TextHelpers::ParseAllNumbers<int64_t>(line);
    }

    ic::Program program2 = program;

    program.inputs.push(1);
    program2.inputs.push(5);

    program.run();
    program2.run();

    return { program.outputs.back(), program2.outputs.back() };
}

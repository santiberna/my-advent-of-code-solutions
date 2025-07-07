#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <utility/int_code.hpp>

REGISTER_CHALLENGE(Y2019_C9, "input/Y2019/C9.txt")
{
    ic::Program program {};

    program.state = { 104, 1125899906842624, 99 };

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto line = TextHelpers::StreamToString(stream.value());
        program.state = TextHelpers::ParseAllNumbers<int64_t>(line);
    }

    int64_t answer1 = 0, answer2 = 0;
    program.state.resize(1'024 * 10);

    {
        auto test = program;
        test.inputs.push(1);
        test.run();
        answer1 = test.outputs.back();
    }

    {
        auto test = program;
        test.inputs.push(2);
        test.run();
        answer2 = test.outputs.back();
    }

    return { answer1, answer2 };
}
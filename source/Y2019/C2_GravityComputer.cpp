#include "utility/timer.hpp"
#include <utility/int_code.hpp>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>

REGISTER_CHALLENGE(Y2019_C2, "input/Y2019/C2.txt")
{
    ic::Program program;
    program.state = { 1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50 };

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto line = TextHelpers::StreamToString(stream.value());
        program.state = TextHelpers::ParseAllNumbers<int64_t>(line);
    }

    auto run1 = program;
    run1.state[1] = 12;
    run1.state[2] = 2;
    run1.run();

    int64_t answer1 = run1.state.at(0);

    int64_t answer2 = -1;

    {
        for (int64_t noun = 0; noun < 100; noun++)
        {
            for (int64_t verb = 0; verb < 100; verb++)
            {
                auto test_run = program;

                test_run.state[1] = noun;
                test_run.state[2] = verb;

                test_run.run();

                auto result = test_run.state.at(0);

                if (result == 19690720)
                {
                    answer2 = noun * 100 + verb;
                }
            }
        }
    }

    return { answer1, answer2 };
}

// {
//         auto test_run = program;

//         for (int64_t noun = 0; noun < 100; noun++)
//         {
//             for (int64_t verb = 0; verb < 100; verb++)
//             {
//                 test_run.state[1] = noun;
//                 test_run.state[2] = verb;

//                 test_run.run();

//                 auto result = test_run.state.at(0);

//                 if (result == 19690720)
//                 {
//                     answer2 = noun * 100 + verb;
//                 }

//                 std::memcpy(test_run.state.data(), program.state.data(), program.state.size() * sizeof(int64_t));
//             }
//         }
//     }
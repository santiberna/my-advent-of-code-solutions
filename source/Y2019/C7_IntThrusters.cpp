#include <algorithm>
#include <utility/int_code.hpp>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <utility/combinations.hpp>
#include <vector>

REGISTER_CHALLENGE(Y2019_C7, "input/Y2019/C7.txt")
{
    ic::Program program {};
    program.state = { 3, 26, 1001, 26, -4, 26, 3, 27, 1002, 27, 2, 27, 1, 27, 26,
        27, 4, 27, 1001, 28, -1, 28, 1005, 28, 6, 99, 0, 0, 5 };

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto line = TextHelpers::StreamToString(stream.value());
        program.state = TextHelpers::ParseAllNumbers<int64_t>(line);
    }

    int64_t answer1 = 0;
    int64_t answer2 = 0;

    {
        std::vector<int> possible_settings = { 0, 1, 2, 3, 4 };
        auto all_permutations = findPermutations(possible_settings, 5);

        for (size_t j = 0; j < all_permutations.getHeight(); j++)
        {
            int64_t intensity = 0;

            for (size_t i = 0; i < all_permutations.getWidth(); i++)
            {
                auto test_program = program;

                test_program.inputs.emplace(all_permutations.at(i, j));
                test_program.inputs.emplace(intensity);

                test_program.run();
                intensity = test_program.outputs.back();
            }

            answer1 = std::max(answer1, intensity);
        }
    }

    {
        std::vector<int> possible_settings = { 5, 6, 7, 8, 9 };
        auto all_permutations = findPermutations(possible_settings, 5);

        for (size_t j = 0; j < all_permutations.getHeight(); j++)
        {
            std::vector<ic::Program> programs {};

            for (size_t i = 0; i < all_permutations.getWidth(); ++i)
            {
                auto new_program = program;
                new_program.inputs.emplace(all_permutations.at(i, j));

                programs.emplace_back(std::move(new_program));
            }

            size_t thruster = 0;
            int64_t intensity = 0;

            while (true)
            {
                auto& step = programs.at(thruster);
                thruster = (thruster + 1) % 5;

                step.inputs.emplace(intensity);

                auto result = step.run();

                if (result == ic::HALT && step.outputs.empty())
                {
                    break;
                }

                intensity = step.outputs.back();
                step.outputs.pop_back();
            }

            answer2 = std::max(answer2, intensity);
        }
    }

    return { answer1, answer2 };
}

// 5619234
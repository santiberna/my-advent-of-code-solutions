#include <utility/challenge_runner.hpp>
#include <iostream>

int main(int argv, const char* args[])
{
    ChallengeRunner::Get().RunAllChallenges(std::cout);
}
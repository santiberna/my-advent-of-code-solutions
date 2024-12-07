#include <common.hpp>
#include <iostream>
#include <vector>

int main(int argv, const char* args[])
{
    ChallengeRunner::Get().RunAllChallenges(std::cout);
}
#include "Utility/TextHelpers.hpp"
#include <common.hpp>
#include <iostream>
#include <vector>

#include <Utility/Array2D.hpp>

int main(int argv, const char* args[])
{
    ChallengeRunner::Get().RunAllChallenges(std::cout);

    // if (auto stream = TextHelpers::OpenFileReadStream("input/Y2024/C16.txt"))
    // {
    //     auto grid = TextHelpers::StreamToGrid(stream.value());
    //     grid.Print();
    // }
}
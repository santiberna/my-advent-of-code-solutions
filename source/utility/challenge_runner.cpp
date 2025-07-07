#include <utility/challenge_runner.hpp>
#include <utility/timer.hpp>
#include <ostream>

void RunChallengeImpl(const std::string& name, std::ostream& out, Challenge* challenge, const Input& input)
{
    Stopwatch t {};
    out << "Running Challenge "
        << name
        << "...\n";

    auto answer = challenge(input);

    out << "Answer Part 1: "
        << answer.first << "\n"
        << "Answer Part 2: "
        << answer.second << "\n";

    out << "Challenge " << name << " complete! "
        << "(Took " << t.GetElapsed().count() << " milliseconds)\n\n";
}

ChallengeRunner& ChallengeRunner::Get()
{
    static ChallengeRunner s {};
    return s;
}

bool ChallengeRunner::RegisterTest(const std::string& name, Challenge* challenge, const Input& input)
{
    challenges_registered.emplace(name, std::make_pair(input, challenge));
    return true;
}

void ChallengeRunner::RunChallenge(const std::string& name, std::ostream& out_result) const
{
    if (auto it = challenges_registered.find(name); it != challenges_registered.end())
    {
        // Runs the challenge
        RunChallengeImpl(it->first, out_result, it->second.second, it->second.first);
    }
}

void ChallengeRunner::RunAllChallenges(std::ostream& out_result) const
{
    for (auto& [name, input_challenge] : challenges_registered)
    {
        // Runs the challenge
        RunChallengeImpl(name, out_result, input_challenge.second, input_challenge.first);
    }
}

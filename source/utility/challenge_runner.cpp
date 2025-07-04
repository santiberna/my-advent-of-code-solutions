#include <utility/challenge_runner.hpp>
#include <utility/timer.hpp>
#include <utility/log.hpp>
#include <ostream>

void RunChallengeImpl(const std::string& name, std::ostream& out, Challenge* challenge, const Input& input)
{
    Stopwatch t {};
    out << Log::Format("Running Challenge {}...\n", name);
    auto answer = challenge(input);
    out << Log::Format("Answer Part 1: {}\n Answer Part 2: {}\n", answer.first, answer.second);
    out << Log::Format("Challenge {} complete! (Took {:.2f} milliseconds)\n\n", name, t.GetElapsed().count());
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

#pragma once
#include <string>
#include <cstdint>
#include <unordered_map>

using Result = std::pair<int64_t, int64_t>; // Most challenges have a big number as an answer, and two parts
using Input = std::string; // Most challenges have a path or string as input
using Challenge = Result(const Input&);

// Doesn't need to be a singleton, but I made it to make registering tests easier
class ChallengeRunner
{
public:
    static ChallengeRunner& Get();

    // Use the exact function name to run the specific challenge
    void RunChallenge(const std::string& name, std::ostream& out_result) const;

    // Run all challenges, we will use the output streams to get our result
    void RunAllChallenges(std::ostream& out_result) const;

    // Combined with a macro to allow adding challenges anywhere in code
    bool RegisterTest(const std::string& name, Challenge* challenge, const Input& input);

private:
    ChallengeRunner() = default;

    using InputChallengePair = std::pair<Input, Challenge*>;
    std::unordered_map<std::string, InputChallengePair> challenges_registered {};
};

#define STRINGIFY(identifier) #identifier
#define CONCATENATE(var, suffix) var##suffix

#define REGISTER_CHALLENGE(func, input_val)                                                                     \
    Result func(const Input&);                                                                                  \
    namespace                                                                                                   \
    {                                                                                                           \
        static const bool discarded = ChallengeRunner::Get().RegisterTest(STRINGIFY(func), &(func), input_val); \
    }                                                                                                           \
    Result func(const Input& input)

#define REGISTER_CHALLENGE_DISABLED(func, input_val) \
    Result func(const Input& input)

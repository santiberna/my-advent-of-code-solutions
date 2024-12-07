#include <Utility/MD5Hash.hpp>
#include <common.hpp>
#include <string>

// This challenge is disabled because it takes too long
REGISTER_CHALLENGE(Y2015_C4_HashingBitcoin, "ckczppom")
{
    MD5_String base_key { input };
    size_t base_key_size = base_key.GetStringLength();

    auto CheckZero = [](const Hash128& hash, size_t index)
    {
        size_t byte = index / 2;
        bool odd = index % 2;

        if (odd)
            return (hash.as_bytes.at(byte) % 16) == 0;
        else
            return (hash.as_bytes.at(byte) >> 4) == 0;
    };

    int64_t answer1 = 0, answer2 = 0;

    // First part
    while (true)
    {
        base_key.Resize(base_key_size);
        base_key.Append(std::to_string(answer1));

        auto hash = MD5Hash(base_key);

        bool check_zeros = CheckZero(hash, 0) && CheckZero(hash, 1) && CheckZero(hash, 2) && CheckZero(hash, 3) && CheckZero(hash, 4);

        if (check_zeros)
            break;

        answer1++;
    }

    // Second part
    while (true)
    {
        base_key.Resize(base_key_size);
        base_key.Append(std::to_string(answer2));

        auto hash = MD5Hash(base_key);

        bool check_zeros = hash.as_bytes[0] == 0 && hash.as_bytes[1] == 0 && hash.as_bytes[2] == 0;
        if (check_zeros)
            break;

        answer2++;
    }

    return { answer1, answer2 };
}
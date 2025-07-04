#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <cassert>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <Utility/Log.hpp>
#include <vector>

int64_t calc_checksum(std::vector<int32_t>& v)
{
    int64_t out {};
    for (int64_t i = 0; i < v.size(); ++i)
    {
        if (v[i] == -1)
            continue;

        out += i * v[i];
    }

    return out;
}

void print(std::vector<int32_t>& v)
{
    for (auto c : v)
    {
        if (c == -1)
            std::cout << '.';
        else
            std::cout << c;
    }

    std::cout << std::endl;
}

REGISTER_CHALLENGE_DISABLED(Y2024_C9, "input/Y2024/C9.txt")
{
    std::string compressed {};

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        compressed = TextHelpers::StreamToString(stream.value());
    }

    std::vector<int32_t> expanded {};
    for (size_t i = 0; i < compressed.size(); i++)
    {
        std::string_view str = { compressed.begin() + i, compressed.begin() + i + 1 };
        auto n = TextHelpers::ParseNumber<int>(str);

        bool empty = (i % 2 == 1);
        int32_t m = empty ? -1 : (i / 2);

        if (n)
            for (int r = 0; r < n.value().first; r++)
            {
                expanded.emplace_back(m);
            }
    }

    int64_t answer1 {}, answer2 {};

    {
        auto reordered = expanded;

        size_t current = 0;
        size_t last = reordered.size() - 1;

        for (; current < last; current++)
        {
            auto val = reordered[current];

            if (val == -1)
            {
                for (size_t check = last; check > current; --check)
                {
                    auto other = reordered[check];

                    if (other != -1)
                    {
                        std::swap(reordered[current], reordered[check]);
                        last = check;
                        break;
                    }
                }
            }
        }

        answer1 = calc_checksum(reordered);
    }

    {
        struct Section
        {
            int32_t id {};
            size_t pos {}, length {};
        };

        std::vector<Section> free_sections {};
        std::vector<Section> occupied_sections {};

        int32_t current_id = 0;
        size_t accum = 0;

        for (size_t i = 0; i < expanded.size(); i++)
        {
            auto next = expanded[i];

            if (next != current_id)
            {
                if (current_id == -1)
                {
                    free_sections.emplace_back(current_id, i - accum, accum);
                }
                else
                {
                    occupied_sections.emplace_back(current_id, i - accum, accum);
                }

                accum = 1;
                current_id = next;
            }
            else
            {
                accum++;
            }
        }

        if (current_id == -1)
        {
            free_sections.emplace_back(current_id, expanded.size() - accum, accum);
        }
        else
        {
            occupied_sections.emplace_back(current_id, expanded.size() - accum, accum);
        }

        for (size_t i = occupied_sections.size() - 1; i > 0; --i)
        {
            auto& section = occupied_sections[i];

            for (size_t j = 0; j < free_sections.size(); ++j)
            {
                auto& free_block = free_sections[j];

                if (free_block.length >= section.length && free_block.pos < section.pos)
                {
                    // Move operation
                    section.pos = free_block.pos;
                    free_block.pos += section.length;
                    free_block.length -= section.length;
                    break;
                }
            }
        }

        auto defragmented = std::vector<int32_t>(expanded.size(), -1);

        for (auto f : occupied_sections)
        {
            for (size_t i = 0; i < f.length; i++)
            {
                defragmented.at(f.pos + i) = f.id;
            }
        }

        answer2 = calc_checksum(defragmented);
    }

    return { answer1, answer2 };
}
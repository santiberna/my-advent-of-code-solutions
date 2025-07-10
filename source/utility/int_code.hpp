#pragma once

#include <vector>
#include <queue>
#include <string>
#include <optional>

namespace ic
{

enum ExitCode : int64_t
{
    HALT,
    OUT_OF_BOUNDS,
    WAITING_INPUT
};

constexpr size_t DEFAULT_MEMORY_REQUIREMENTS = 1024 * 256; // 256 KB

struct Program
{
    static std::optional<Program> fromFile(const std::string& path, size_t memory_req = DEFAULT_MEMORY_REQUIREMENTS);

    std::vector<int64_t> state {};
    std::vector<int64_t> outputs {};
    std::queue<int64_t> inputs {};
    int64_t instruction_ptr = 0;
    int64_t relative_offset = 0;

    ExitCode run();
};

}
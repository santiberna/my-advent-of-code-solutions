#pragma once
#include <cstdint>
#include <vector>
#include <queue>
#include <stack>

namespace ic
{

enum OpCode : int64_t
{
    NONE = 0,
    ADD = 1,
    MULTIPLY = 2,
    MEMSET = 3,
    RET = 4,
    JIT = 5,
    JIF = 6,
    LESS = 7,
    EQ = 8,
    REL_OFF = 9,
    STOP = 99
};

enum AddressMode : int64_t
{
    ADDRESS,
    IMMEDIATE,
    RELATIVE
};

enum ExitCode : int64_t
{
    HALT,
    OUT_OF_BOUNDS,
    WAITING_INPUT
};

struct Operation
{
    OpCode code {};
    int64_t stride {};
    AddressMode parameter_mode1 {};
    AddressMode parameter_mode2 {};
    AddressMode parameter_mode3 {};
};

struct Program
{
    std::vector<int64_t> state {};
    std::vector<int64_t> outputs {};
    std::queue<int64_t> inputs {};
    int64_t instruction_ptr = 0;
    int64_t relative_offset = 0;

    ExitCode run();
};

}
#include "int_code.hpp"
#include <utility/int_code.hpp>

using namespace ic;

int64_t getAddress(const Program& program, int64_t offset, ic::AddressMode mode)
{
    switch (mode)
    {
    case ic::AddressMode::ADDRESS:
    {
        return program.state.at(program.instruction_ptr + offset);
    }
    case ic::AddressMode::RELATIVE:
    {
        return program.state.at(program.instruction_ptr + offset) + program.relative_offset;
    }
    default:
        throw;
    }
}

int64_t getOperand(const Program& program, int64_t offset, ic::AddressMode mode)
{
    switch (mode)
    {
    case ic::AddressMode::ADDRESS:
    {
        auto address = program.state.at(program.instruction_ptr + offset);
        return program.state.at(address);
    }
    case ic::AddressMode::IMMEDIATE:
    {
        return program.state.at(program.instruction_ptr + offset);
    }
    case ic::AddressMode::RELATIVE:
    {
        auto address = program.state.at(program.instruction_ptr + offset);
        return program.state.at(address + program.relative_offset);
    }
    default:
        throw;
    }
}

Operation makeOperation(int64_t operation)
{
    Operation out {};

    out.code = (OpCode)(operation % 100);

    if (out.code == MULTIPLY || out.code == ADD || out.code == LESS || out.code == EQ)
    {
        out.stride = 4;
    }
    else if (out.code == MEMSET || out.code == RET || out.code == REL_OFF)
    {
        out.stride = 2;
    }

    out.parameter_mode1 = (AddressMode)((operation / 100) % 10);
    out.parameter_mode2 = (AddressMode)((operation / 1000) % 10);
    out.parameter_mode3 = (AddressMode)((operation / 10000) % 10);

    return out;
}

void executeOperation(Program& program, const Operation& operation)
{
    switch (operation.code)
    {
    case MULTIPLY:
    {
        int64_t operand1 = getOperand(program, 1, operation.parameter_mode1);
        int64_t operand2 = getOperand(program, 2, operation.parameter_mode2);
        int64_t operand3 = getAddress(program, 3, operation.parameter_mode3);

        program.state.at(operand3) = operand1 * operand2;
        break;
    }
    case ADD:
    {
        int64_t operand1 = getOperand(program, 1, operation.parameter_mode1);
        int64_t operand2 = getOperand(program, 2, operation.parameter_mode2);
        int64_t operand3 = getAddress(program, 3, operation.parameter_mode3);

        program.state.at(operand3) = operand1 + operand2;
        break;
    }
    case MEMSET:
    {
        int64_t operand1 = getAddress(program, 1, operation.parameter_mode1);

        program.state.at(operand1) = program.inputs.front();
        program.inputs.pop();

        break;
    }
    case RET:
    {
        int64_t operand1 = getOperand(program, 1, operation.parameter_mode1);
        program.outputs.emplace_back(operand1);
        break;
    }
    case JIT:
    {
        int64_t operand1 = getOperand(program, 1, operation.parameter_mode1);
        int64_t operand2 = getOperand(program, 2, operation.parameter_mode2);
        if (operand1 != 0)
        {
            program.instruction_ptr = operand2;
        }
        else
        {
            program.instruction_ptr += 3;
        }
        break;
    }
    case JIF:
    {
        int64_t operand1 = getOperand(program, 1, operation.parameter_mode1);
        int64_t operand2 = getOperand(program, 2, operation.parameter_mode2);

        if (operand1 == 0)
        {
            program.instruction_ptr = operand2;
        }
        else
        {
            program.instruction_ptr += 3;
        }
        break;
    }
    case LESS:
    {
        int64_t operand1 = getOperand(program, 1, operation.parameter_mode1);
        int64_t operand2 = getOperand(program, 2, operation.parameter_mode2);
        int64_t operand3 = getAddress(program, 3, operation.parameter_mode3);

        program.state.at(operand3) = operand1 < operand2;
        break;
    }
    case EQ:
    {
        int64_t operand1 = getOperand(program, 1, operation.parameter_mode1);
        int64_t operand2 = getOperand(program, 2, operation.parameter_mode2);
        int64_t operand3 = getAddress(program, 3, operation.parameter_mode3);

        program.state.at(operand3) = operand1 == operand2;
        break;
    }
    case REL_OFF:
    {
        int64_t operand1 = getOperand(program, 1, operation.parameter_mode1);
        program.relative_offset += operand1;
        break;
    }
    default:
        throw;
    }

    program.instruction_ptr += operation.stride;
}

ExitCode ic::Program::run()
{
    while (true)
    {
        if (instruction_ptr >= (int64_t)state.size())
        {
            return ExitCode::OUT_OF_BOUNDS;
        }

        auto op_code = makeOperation(state.at(instruction_ptr));

        if (op_code.code == OpCode::STOP)
        {
            return ExitCode::HALT;
        }

        if (op_code.code == OpCode::MEMSET && inputs.empty())
        {
            return ExitCode::WAITING_INPUT;
        }

        executeOperation(*this, op_code);
    }
}

#ifndef ENIGMA_MANAGER
#define ENIGMA_MANAGER

#include "../memory/EnigmaMemory.hpp"
/*
This is like an OS that will maintain the Enigma VM, it's execution, program loading and program execution
*/

namespace Manager
{
    static std::vector<std::uint64_t> buffer; // the I/O buffer

    // load the instructions into instruction memory
    inline void load_instructions(std::vector<qword> &instructions);

    // load the data(8-bit)
    inline void load_data8(std::vector<qword> &data);
    inline void load_data8(qword data);

    // load the data(16-bit)
    inline void load_data16(std::vector<qword> &data);
    inline void load_data16(qword data);

    // load the data(32-bit)
    inline void load_data32(std::vector<qword> &data);
    inline void load_data32(qword data);

    // load the data(64-bit)
    inline void load_data64(std::vector<qword> &data);
    inline void load_data64(qword data);

    inline void handlesyscalls();

    inline void start_execution();
};

#include "../CPU/EnigmaCPU.hpp"
#include "EnigmaSyscalls.hpp"

void Manager::load_instructions(std::vector<qword> &instructions)
{
    if (instructions.size() > 1024)
    {
        // first make sure that all of the instructions can be taken
        CPU::instruction_memory.pointer_limit_increase(instructions.size() - 1024);
        CPU::instruction_memory.resize(instructions.size());
    }
    qword mem_addr = CPU::mem_pointer;
    for (auto x : instructions)
    {
        CPU::instruction_memory.mem_write64(mem_addr, x);
        mem_addr += 8;
    }
    CPU::mem_pointer = mem_addr;
}

void Manager::load_data8(std::vector<qword> &data)
{
    if (data.size() + 255 > 1024)
    {
        CPU::data_memory.pointer_limit_increase(data.size() - 1024 - 255);
        CPU::data_memory.resize(data.size() - 1024 - 255);
    }
    qword mem_addr = CPU::mem_pointer;
    for (auto x : data)
    {
        CPU::data_memory.mem_write8(mem_addr, x);
        mem_addr++;
    }
    CPU::mem_pointer = mem_addr;
}

void Manager::load_data16(std::vector<qword> &data)
{
    if (data.size() + 255 > 1024)
    {
        CPU::data_memory.pointer_limit_increase(data.size() - 1024 - 255);
        CPU::data_memory.resize(data.size() - 1024 - 255);
    }
    qword mem_addr = CPU::mem_pointer;
    for (auto x : data)
    {
        CPU::data_memory.mem_write16(mem_addr, x);
        mem_addr += 2;
    }
    CPU::mem_pointer = mem_addr;
}

void Manager::load_data32(std::vector<qword> &data)
{
    if (data.size() + 255 > 1024)
    {
        CPU::data_memory.pointer_limit_increase(data.size() - 1024 - 255);
        CPU::data_memory.resize(data.size() - 1024 - 255);
    }
    qword mem_addr = CPU::mem_pointer;
    for (auto x : data)
    {
        CPU::data_memory.mem_write32(mem_addr, x);
        mem_addr += 4;
    }
    CPU::mem_pointer = mem_addr;
}

void Manager::load_data64(std::vector<qword> &data)
{
    if (data.size() + 255 > 1024)
    {
        CPU::data_memory.pointer_limit_increase(data.size() - 1024 - 255);
        CPU::data_memory.resize(data.size() - 1024 - 255);
    }
    qword mem_addr = CPU::mem_pointer;
    for (auto x : data)
    {
        CPU::data_memory.mem_write64(mem_addr, x);
        mem_addr += 8;
    }
    CPU::mem_pointer = mem_addr;
}

void Manager::handlesyscalls()
{
    switch (CPU::_registers[CPU::ar])
    {
    case 0:
        Syscalls::sysMemIncrease();
        break;
    case 1:
        Syscalls::sysUpperLimitIncrease();
        break;
    case 2:
        Syscalls::sysIncrPointerLim();
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        break;
    // case 11:
    //     Syscalls::sysRead();
    //     break;
    // case 12:
    //     Syscalls::sysReadBySize();
    //     break;
    // case 13:
    //     Syscalls::sysReadAndSave();
    //     break;
    // case 14:
    //     Syscalls::sysReadBySizeandSave();
    //     break;
    // case 15:
    //     Syscalls::sysReadasFloat();
    //     break;
    // case 16:
    //     Syscalls::sysReadAsNeg();
    //     break;
    // case 17:
    //     Syscalls::sysDisplay();
    //     break;
    // case 18:
    //     Syscalls::sysDisplayAsChar();
    //     break;
    // case 19:
    //     Syscalls::sysDisplayAsFloats();
    //     break;
    // case 20:
    //     Syscalls::sysDisplayAsNeg();
    //     break;
    }
}

void Manager::start_execution()
{
    CPU::run();
}

void Manager::load_data8(qword data)
{
    CPU::data_memory.mem_write8(CPU::mem_pointer, data & 255);
    CPU::mem_pointer++;
}

void Manager::load_data16(qword data)
{
    CPU::data_memory.mem_write16(CPU::mem_pointer, data & 65535);
    CPU::mem_pointer += 2;
}

void Manager::load_data32(qword data)
{
    CPU::data_memory.mem_write32(CPU::mem_pointer, data & 4294967295);
    CPU::mem_pointer += 4;
}

void Manager::load_data64(qword data)
{
    CPU::data_memory.mem_write64(CPU::mem_pointer, data);
    CPU::mem_pointer += 8;
}

#endif
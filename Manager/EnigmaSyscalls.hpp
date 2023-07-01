#ifndef ENIGMA_SYSCALLS
#define ENIGMA_SYSCALLS

#include "../CPU/EnigmaInstructions.hpp"
#include <cmath>

namespace Syscalls
{
    // this increases the data memory size
    // params:
    // ar = 0, br = new size
    inline void sysMemIncrease()
    {
        auto __increse_by = CPU::_registers[CPU::br];
        CPU::data_memory.pointer_limit_increase(__increse_by);
        CPU::data_memory.resize(CPU::data_memory.current_size());
    }

    // increase the upper limit
    // ar = 1
    // br = increase by
    inline void sysUpperLimitIncrease()
    {
        auto __increse_by = CPU::_registers[CPU::br];
        CPU::data_memory.increase_upper_limit(__increse_by);
    }

    // increase the pointer limit
    // this also resizes the memory
    // but both are done by the same function
    // ar = 2
    // br = increase by
    inline void sysIncrPointerLim()
    {
        auto __incr_by = CPU::_registers[CPU::br];
        CPU::data_memory.add_size(__incr_by);
    }

    // calls from 3 to 10 have been reserved for more operations
    // ar = 11
    // br = exit code
    inline void sysExit()
    {
        CPU::running = false;
        CPU::_registers[CPU::ar] = CPU::_registers[CPU::br];
    }

    // ar = 12
    // br = memory address to store the read data
    inline void sysReadNum()
    {
        std::uint64_t in;
        std::cin >> in;
        auto mapped = map_mem(CPU::_registers[CPU::br]);
        switch (mapped.first)
        {
        case 1:
            CPU::data_memory.mem_write8(mapped.second, in);
            break;
        case 2:
            CPU::data_memory.mem_write16(mapped.second, in);
            break;
        case 4:
            CPU::data_memory.mem_write32(mapped.second, in);
            break;
        case 8:
            CPU::data_memory.mem_write64(mapped.second, in);
            break;
        }
    }

    // ar = 13
    // br = memory address to store
    // cr = length of characters to be read
    inline void sysReadChar()
    {
        //this is not how it should be
        auto mapped = map_mem(CPU::_registers[CPU::br]);
        for (std::uint64_t i = 0; i < CPU::_registers[CPU::cr]; i++)
        {
            char in;
            std::cin >> in;
            CPU::data_memory.mem_write8(mapped.second + i, (int)in);
        }
    }

    // ar = 14
    // br = memory address
    inline void sysReadFloat()
    {
        std::string in;
        std::cin >> in;
        auto mapped = map_mem(CPU::_registers[CPU::br]);
        switch (mapped.first)
        {
        case 4:
            CPU::data_memory.mem_write32(mapped.second, strtoDec32(in));
            break;
        case 8:
            CPU::data_memory.mem_write64(mapped.second, strtoDec64(in));
            break;
        default:
            std::cerr << "Error: Float implementation only supports 4 bytes or 8 bytes." << std::endl;
            exit(-1);
        }
    }

    // ar = 15
    // br = address
    inline void sysWriteNum()
    {
        auto mapped = map_mem(CPU::_registers[CPU::br]);
        std::uint64_t val;
        switch (mapped.first)
        {
        case 1:
            val = CPU::data_memory.mem_read8(mapped.second);
            break;
        case 2:
            val = CPU::data_memory.mem_read16(mapped.second);
            break;
        case 4:
            val = CPU::data_memory.mem_read32(mapped.second);
            break;
        case 8:
            val = CPU::data_memory.mem_read64(mapped.second);
            break;
        }
        if ((val >> 63) == 1)
        {
            std::cout << '-' << reverse_complement(val);
        }
        else
        {
            std::cout << val;
        }
    }

    // ar = 16
    // br = address
    // cr = number of characters to write
    inline void sysWriteChar()
    {
        auto mapped = map_mem(CPU::_registers[CPU::br]);
        for (std::uint64_t i = 0; i < CPU::_registers[CPU::cr]; i++)
        {
            std::cout << (char)CPU::data_memory.mem_read8(mapped.second + i);
        }
    }

    // ar = 17
    // br = memory address
    inline void sysWriteFloat()
    {
        auto mapped = map_mem(CPU::_registers[CPU::br]);
        std::uint64_t num;
        switch (mapped.first)
        {
        case 4:
            num = CPU::data_memory.mem_read32(CPU::_registers[CPU::br]);
            std::cout << (num >> 15) << '.' << (num & 0b1111111111111111);
            break;
        case 8:
            num = CPU::data_memory.mem_read64(CPU::_registers[CPU::br]);
            std::cout << (num >> 33) << '.' << (num & 0b11111111111111111111111111111111);
            break;
        default:
            std::cerr << "Error: Float implementation only supports 4 bytes or 8 bytes." << std::endl;
            exit(-1);
        }
    }
   
};

#endif
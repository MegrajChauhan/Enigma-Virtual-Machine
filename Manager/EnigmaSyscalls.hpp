#ifndef ENIGMA_SYSCALLS
#define ENIGMA_SYSCALLS

#include "../memory/EnigmaMemory.hpp"
#include "../CPU/EnigmaCPU.hpp"
#include <cmath>

static std::uint64_t sign_Ext(std::uint64_t toext, int ext_bit)
{
    if (ext_bit == 1)
    {
        return toext & 0b1111111111111111111111111111111111111111111111111111111111111111;
    }
    return toext;
}

static std::uint64_t twoComplement(std::uint64_t _bin)
{
    return ~(_bin) + 1;
}

static std::uint64_t reverse_complement(std::uint64_t torev)
{
    return ~(torev - 1);
}

static std::uint64_t strtoNum(std::string str)
{
    std::uint64_t res = 0;
    int pow = str.length();
    bool is_neg = false;
    if (str[0] == '-')
    {
        is_neg = true;
    }
    for (char x : str)
    {
        switch (x)
        {
        case '1':
            res = res * std::pow(10, pow) + 1;
            break;
        case '2':
            res = res * std::pow(10, pow) + 2;
            break;
        case '3':
            res = res * std::pow(10, pow) + 3;
            break;
        case '4':
            res = res * std::pow(10, pow) + 4;
            break;
        case '5':
            res = res * std::pow(10, pow) + 5;
            break;
        case '6':
            res = res * std::pow(10, pow) + 6;
            break;
        case '7':
            res = res * std::pow(10, pow) + 7;
            break;
        case '8':
            res = res * std::pow(10, pow) + 8;
            break;
        case '9':
            res = res * std::pow(10, pow) + 9;
            break;
        case '0':
            res = res * std::pow(10, pow);
            break;
        case '-':
            pow++;
            break;
        }
        pow--;
    }
    if (is_neg)
    {
        res = twoComplement(sign_Ext(res, 1));
    }
    else
    {
        res = sign_Ext(res, 0);
    }
    return res;
}

static float strtoDec32(std::string num)
{
    std::uint32_t res;
    res = strtoNum(num.substr(0, num.find_first_of('.')));
    std::string dec = num.substr(num.find_first_of('.') + 1);
    res = ((res << 1) | 1) << 16;
    res |= (strtoNum(dec));
    return res;
}

double strtoDec64(std::string num)
{
    std::uint64_t res;
    res = strtoNum(num.substr(0, num.find_first_of('.')));
    std::string dec = num.substr(num.find_first_of('.'));
    res = ((res << 1) | 1) << 32;
    res &= (strtoNum(dec));
    return res;
}

// above functions copied from assembler's parser

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

    // more calling numbers from 3 to 10 have been saved for potential sys functions increase in the future
    // the most basic input operation
    // asks for input[just one character]
    // ar = 11 [saved to buffer]
    inline void sysRead()
    {
        std::uint64_t in;
        std::cin >> in;
        Manager::buffer.push_back(in);
    }

    // another way for taking input
    // asks for input[for the specified number of characters]
    // ar = 12
    // br = read size
    // this is also saved to the buffer
    inline void sysReadBySize()
    {
        for (int i = 0; i < CPU::_registers[CPU::br]; i++)
        {
            sysRead();
        }
    }

    // take input and save it to the specified memory location
    // ar = 13
    // br = memory address
    // just one character
    inline void sysReadAndSave()
    {
        std::uint64_t in;
        std::cin >> in;
        CPU::data_memory.mem_write8(CPU::_registers[CPU::br], in);
    }

    // take input upto a specified size and save it to the specified memory
    // ar = 14
    // br = memory address
    // cr = input length
    inline void sysReadBySizeandSave()
    {
        for (int i = 0; i < CPU::_registers[CPU::cr]; i++)
        {
            sysReadAndSave();
            CPU::_registers[CPU::br]++;
        }
    }

    // for reading floating point numbers
    // and the number must be stored in memory
    // ar = 15
    // br = memory address
    // cr = size[either 4 or 8]
    inline void sysReadasFloat()
    {
        // for those who wonder:
        // if the input float is incorrect, 0 is stored in memory directly
        // there will be no error messages.
        std::string in;
        std::cin >> in;
        if (CPU::_registers[CPU::cr] == 4)
        {
            CPU::data_memory.mem_write32(CPU::_registers[CPU::br], strtoDec32(in));
        }
        else if (CPU::_registers[CPU::cr] == 8)
        {
            CPU::data_memory.mem_write64(CPU::_registers[CPU::br], strtoDec64(in));
        }
    }

    // for negative numbers
    // ar = 16
    // br = memory address
    // cr = size
    inline void sysReadAsNeg()
    {
        std::string in;
        std::cin >> in;
        switch (CPU::_registers[CPU::cr])
        {
        case 1:
            CPU::data_memory.mem_write8(CPU::_registers[CPU::br], strtoNum(in));
            break;
        case 2:
            CPU::data_memory.mem_write16(CPU::_registers[CPU::br], strtoNum(in));
            break;
        case 4:
            CPU::data_memory.mem_write32(CPU::_registers[CPU::br], strtoNum(in));
            break;
        case 8:
            CPU::data_memory.mem_write64(CPU::_registers[CPU::br], strtoNum(in));
            break;
        }
    }

    // The reason for making specific functions for each function instead of providing modes through other registers
    // is to make it easier to maintain

    // for most basic output
    // display a number from the top of the buffer or from a memory address
    // ar = 17
    // br = 0/1
    // cr = memory address if br = 0
    // this displays the everything as numbers since everything is stored as a number
    inline void sysDisplay()
    {
        if (Manager::buffer.size() == 0)
        {
            return;
        }
        if (CPU::_registers[CPU::br] == 1)
        {
            std::cout << Manager::buffer[0];
            Manager::buffer.erase(Manager::buffer.begin());
        }
        else
        {
            std::cout << CPU::data_memory.mem_read8(CPU::_registers[CPU::br]);
        }
    }

    // display as character
    // ar = 18
    // br = 0/1
    // cr = memory address if br = 0
    inline void sysDisplayAsChar()
    {
        if (Manager::buffer.size() == 0)
        {
            return;
        }
        if (CPU::_registers[CPU::br] == 1)
        {
            std::cout << (char)Manager::buffer[0];
            Manager::buffer.erase(Manager::buffer.begin());
        }
        else
        {
            std::cout << (char)CPU::data_memory.mem_read8(CPU::_registers[CPU::br]);
        }
    }

    // display as decimal numbers
    // ar = 19
    // br = memory address
    // cr = size[what is the size of the decimal point? 4 bytes or 8 bytes]
    inline void sysDisplayAsFloats()
    {
        if (CPU::_registers[CPU::cr] == 4)
        {
            auto num = CPU::data_memory.mem_read32(CPU::_registers[CPU::br]);
            std::cout << (num >> 15) << '.' << (num & 0b1111111111111111);
        }
        else
        {
            auto num = CPU::data_memory.mem_read64(CPU::_registers[CPU::br]);
            std::cout << (num >> 33) << '.' << (num & 0b11111111111111111111111111111111);
        }
    }

    // display negative numbers
    // ar = 20
    // br = memory address
    // cr = memory size
    inline void sysDisplayAsNeg()
    {
        //  std::cout<<"-"<<reverse_complement(CPU::data_memor)
        switch (CPU::_registers[CPU::cr])
        {
        case 1:
            std::cout<<"-"<<CPU::data_memory.mem_read8(CPU::_registers[CPU::br]);
            break;
        case 2:
            std::cout<<"-"<<CPU::data_memory.mem_read16(CPU::_registers[CPU::br]);
            break;
        case 4:
            std::cout<<"-"<<CPU::data_memory.mem_read32(CPU::_registers[CPU::br]);
            break;
        case 8:
            std::cout<<"-"<<CPU::data_memory.mem_read64(CPU::_registers[CPU::br]);
            break;
        }
    }
};

#endif
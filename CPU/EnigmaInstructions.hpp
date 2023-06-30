#ifndef ENIGMA_INSTRUCTIONS
#define ENIGMA_INSTRUCTIONS

// this is where all of the instructions and how they work will be defined
//  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
// because of the number of registers, all of them can be represented by 3 bits
// because 3 bits cannot address more than 7 registers, it is impossible for users
// to address sp and pc and cause mayhem

#ifndef ENIGMA_CPU
#include "EnigmaCPU.hpp"
#endif

namespace InstructionsImpl
{
    // first with basic instructions such as conditional operations
    void cmp();
    void jmp();
    void jz();
    void jnz();
    void jn();
    void jnn();
    void je();
    void jne();
    void jg();
    void jge();
    void js();
    void jse();
};

/*
this function basically compares the values at given registers and
does every possible operation that could trigger a specific flag then it exits
*/
void InstructionsImpl::cmp()
{
    // 000000 00 00000000 00000000 00000000 00000000 00000000 00000000 00 000 000
    // in the given instruction, first 6 are for the instruction itself
    // because the instruction only works on registers, the last 6 bits will be used for the operands leaving other bits reserved
    std::uint64_t reg1 = CPU::_registers[CPU::instr & 3UL];        // get the last register
    std::uint64_t reg2 = CPU::_registers[(CPU::instr >> 3 & 3UL)]; // get the first register
    // start comparing
    if (reg1 == reg2 == 0)
    {
        CPU::flags[CPU::ZERO] = 1; // 1 means yes and 0 means no
        CPU::flags[CPU::NONZERO] = 0;
    }
    else
    {
        CPU::flags[CPU::ZERO] = 0; // may have been turned 1 by some other operation
        CPU::flags[CPU::NONZERO] = 1;
    }
    if (reg1 == reg2)
    {
        CPU::flags[CPU::EQUAL] = 1;
        CPU::flags[CPU::NOT_EQ] = 0;
    }
    else
    {
        CPU::flags[CPU::EQUAL] = 0;
        CPU::flags[CPU::NOT_EQ] = 1;
    }
    if (reg2 > reg1)
    {
        // because reg2 is actually the first operand
        CPU::flags[CPU::GREATER] = 1;
        CPU::flags[CPU::GREATER_EQ] = 0;
        CPU::flags[CPU::SMALLER] = 0;
        CPU::flags[CPU::SMALLER_EQ] = 0;
    }
    if (reg2 >= reg1)
    {
        CPU::flags[CPU::GREATER] = 1;
        CPU::flags[CPU::GREATER_EQ] = 1;
        CPU::flags[CPU::SMALLER] = 0;
        CPU::flags[CPU::SMALLER_EQ] = 0;
    }
    if (reg2 < reg1)
    {
        CPU::flags[CPU::GREATER] = 0;
        CPU::flags[CPU::GREATER_EQ] = 0;
        CPU::flags[CPU::SMALLER] = 1;
        CPU::flags[CPU::SMALLER_EQ] = 0;
    }
    if (reg2 <= reg1)
    {
        CPU::flags[CPU::GREATER] = 0;
        CPU::flags[CPU::GREATER_EQ] = 0;
        CPU::flags[CPU::SMALLER] = 1;
        CPU::flags[CPU::SMALLER_EQ] = 1;
    }
}

/*
jump to the memory location specified in the instruction memory only,
the program counter is set to that location and so the only operands
the jmp instructions take is memory address and nothing else
*/
void InstructionsImpl::jmp()
{
    //  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    // first 6 bits for instruction and since we have a 64-bit addresses,
    // all other bits here are reserved and the actual destination is in the next address
    CPU::fetch(); // update the memory
    // since we have implemented in the assembler, the first 6 bits of an address actually tell us
    // about the length of the address but since we are working with instructions that is irrelevent
    CPU::_registers[CPU::pc] = CPU::instr; // update the counter
}

//all of the conditional jumps basically call the jmp function only when an condition is fulfilled

void InstructionsImpl::jz()
{
    //if zero flag is set, jmp
    if(CPU::flags[CPU::ZERO] == 1)
    {jmp();}
}

void InstructionsImpl::jnz()
{
    //if zero flag is not set, jmp
    if(CPU::flags[CPU::ZERO] != 1)
    {jmp();}
}

void InstructionsImpl::jn()
{
    //if negative flag is set, jmp
    if(CPU::flags[CPU::NEGATIVE] == 1)
    {jmp();}
}

void InstructionsImpl::jnn()
{
    //if negative flag is not set, jmp
    if(CPU::flags[CPU::NEGATIVE] != 1)
    {jmp();}
}

void InstructionsImpl::je()
{
    //if equal flag is set, jmp
    if(CPU::flags[CPU::EQUAL] == 1)
    {jmp();}
}

void InstructionsImpl::jne()
{
    //if equal flag is not set, jmp
    if(CPU::flags[CPU::EQUAL] != 1)
    {jmp();}
}

void InstructionsImpl::jg()
{
    //if greater than flag is set, jmp
    if(CPU::flags[CPU::GREATER] == 1)
    {jmp();}
}

void InstructionsImpl::jge()
{
    //if greater equal flag or greater flag is set, jmp
    if(CPU::flags[CPU::GREATER_EQ] == 1 || CPU::flags[CPU::GREATER] == 1 )
    {jmp();}
}

void InstructionsImpl::js()
{
    //if smaller than flag is set, jmp
    if(CPU::flags[CPU::SMALLER] == 1)
    {jmp();}
}

void InstructionsImpl::jse()
{
    //if smaller equal flag or smaller flag is set, jmp
    if(CPU::flags[CPU::SMALLER_EQ] == 1 || CPU::flags[CPU::SMALLER] == 1 )
    {jmp();}
}
//after this the assembler was updated
#endif
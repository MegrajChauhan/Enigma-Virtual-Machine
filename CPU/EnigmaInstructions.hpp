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
    void je();
    void jne();
    void jg();
    void jge();
    void js();
    void jse();

    // move instructions
    void mov();
    void movzx();
    void movsx();
    void store();
    void load();
    void lea();
    void push();
    void pop();
    void pushr();
    void popr();
    void movz();
    void movnz();
    void move();
    void movne();
    void movg();
    void movge();
    void movs();
    void movse();

    // logical operations
    void iand();
    void inot();
    void ior();
    void ixor();
    void lshift();
    void rshift();

    // arithmetic operations
    void add();
    void sub();
    void mul();
    void div();
    void inc();
    void dec();
    void neg();

};

std::pair<std::uint8_t, std::uint64_t> map_mem(std::uint64_t addr)
{
    // unlike the memory where memory addressing is direct, it uses a format here,
    // 0000 0000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    // in a 64-bit address like this, first 4 bits will usually hold the size for the memory block in the memory(1 byte, 2 byte, 4 or 8?)
    // then the remaining is the actual memory address
    return std::make_pair((addr >> 60) & 15, addr & 0xFFFFFFFFFFFFFFF);
}

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

// all of the conditional jumps basically call the jmp function only when an condition is fulfilled

void InstructionsImpl::jz()
{
    // if zero flag is set, jmp
    if (CPU::flags[CPU::ZERO] == 1)
    {
        jmp();
    }
}

void InstructionsImpl::jnz()
{
    // if zero flag is not set, jmp
    if (CPU::flags[CPU::ZERO] != 1)
    {
        jmp();
    }
}

void InstructionsImpl::je()
{
    // if equal flag is set, jmp
    if (CPU::flags[CPU::EQUAL] == 1)
    {
        jmp();
    }
}

void InstructionsImpl::jne()
{
    // if equal flag is not set, jmp
    if (CPU::flags[CPU::EQUAL] != 1)
    {
        jmp();
    }
}

void InstructionsImpl::jg()
{
    // if greater than flag is set, jmp
    if (CPU::flags[CPU::GREATER] == 1)
    {
        jmp();
    }
}

void InstructionsImpl::jge()
{
    // if greater equal flag or greater flag is set, jmp
    if (CPU::flags[CPU::GREATER_EQ] == 1 || CPU::flags[CPU::GREATER] == 1)
    {
        jmp();
    }
}

void InstructionsImpl::js()
{
    // if smaller than flag is set, jmp
    if (CPU::flags[CPU::SMALLER] == 1)
    {
        jmp();
    }
}

void InstructionsImpl::jse()
{
    // if smaller equal flag or smaller flag is set, jmp
    if (CPU::flags[CPU::SMALLER_EQ] == 1 || CPU::flags[CPU::SMALLER] == 1)
    {
        jmp();
    }
}

void InstructionsImpl::mov()
{
    //  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    //  basically mov can be used to move value from another register to the destination register
    //  or an immediate value or an address which is being held by another register
    //  or the value at the address being held by another register
    //  first 6 bits for instruction itself, 2 bits for what format to use, the remaining for operands
    short format = (CPU::instr >> 56) & 3UL; // get the format type
    switch (format)
    {
    case 0:
    {
        // for register-register mov: 00
        //  000000 00 00000000 00000000 00000000 00000000 00000000 00000000 00 000 000
        // last 6 bits for the operands
        CPU::_registers[(CPU::instr >> 3) & 7UL] = CPU::_registers[CPU::instr & 7UL]; // put the value
        break;
    }
    case 1:
    {
        // for register-imm mov: 01
        //  000000 01 00000000 00000000 00000000 00000000 00000000 00000000 00000 000
        // last 3 bits for the destination register and the remaining bits for the immediate value
        // the immediate can be as large as 2^53 bits which is very large for an immediate
        CPU::_registers[(CPU::instr & 7UL)] = (CPU::instr >> 3) & 0x1FFFFFFFFFFFFF; // get the immediate value
        break;
    }
    case 2:
    {
        // for register-addr mov: 10
        //  000000 10 00000000 00000000 00000000 00000000 00000000 00000000 00 000 000
        // last 6 bits are for the operand registers
        // this is the same as case 0, why? for clear distinction, to know that the value being handled is clearly an address
        // you can use 00 as well which does the same
        CPU::_registers[(CPU::instr >> 3) & 7UL] = CPU::_registers[CPU::instr & 7UL]; // put the address
        break;
    }
    case 3:
    {
        // for register-register[addr] mov: 11
        //  000000 10 00000000 00000000 00000000 00000000 00000000 00000000 00 000 000
        // this operation moves the value at the address at the source register and saves it in the destination register
        // the address should be loaded into the register first
        auto mapped = map_mem(CPU::_registers[CPU::instr & 3UL]);
        if (mapped.first == 1)
        {
            CPU::_registers[(CPU::instr >> 3) & 3UL] = CPU::data_memory.mem_read8(mapped.second);
        }
        else if (mapped.first == 2)
        {
            CPU::_registers[(CPU::instr >> 3) & 3UL] = CPU::data_memory.mem_read16(mapped.second);
        }
        else if (mapped.first == 4)
        {
            CPU::_registers[(CPU::instr >> 3) & 3UL] = CPU::data_memory.mem_read32(mapped.second);
        }
        else if (mapped.first == 8)
        {
            CPU::_registers[(CPU::instr >> 3) & 3UL] = CPU::data_memory.mem_read64(mapped.second);
        }
        break;
    }
    }
}

void InstructionsImpl::movzx()
{
    // 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    // this instruction will zero extend any value and then only put it in the destination register
    // movzx is exactly the same to mov and everything is the same except, it will zero extend the values
    // regardless of their signs[copied the following from mov]
    short format = (CPU::instr >> 56) & 3UL; // get the format type
    switch (format)
    {
    case 0:
    {
        // for register-register mov: 00
        //  000000 00 00000000 00000000 00000000 00000000 00000000 00000000 00 000 000
        // last 6 bits for the operands
        CPU::_registers[(CPU::instr >> 3) & 7UL] = zero_Ext(CPU::_registers[CPU::instr & 7UL]); // put the value
        break;
    }
    case 1:
    {
        // for register-imm mov: 01
        //  000000 01 00000000 00000000 00000000 00000000 00000000 00000000 00000 000
        // last 3 bits for the destination register and the remaining bits for the immediate value
        // the immediate can be as large as 2^53 bits which is very large for an immediate
        CPU::_registers[(CPU::instr & 7UL)] = zero_Ext((CPU::instr >> 3) & 0x1FFFFFFFFFFFFF); // get the immediate value
        break;
    }
    case 2:
    {
        // for register-addr mov: 10
        //  000000 10 00000000 00000000 00000000 00000000 00000000 00000000 00 000 000
        // last 6 bits are for the operand registers
        // this is the same as case 0, why? for clear distinction, to know that the value being handled is clearly an address
        // you can use 00 as well which does the same
        CPU::_registers[(CPU::instr >> 3) & 7UL] = zero_Ext(CPU::_registers[CPU::instr & 7UL]); // put the address
        break;
    }
    case 3:
    {
        // for register-register[addr] mov: 11
        //  000000 10 00000000 00000000 00000000 00000000 00000000 00000000 00 000 000
        // this operation moves the value at the address at the source register and saves it in the destination register
        // the address should be loaded into the register first
        auto mapped = map_mem(CPU::_registers[CPU::instr & 3UL]);
        if (mapped.first == 1)
        {
            CPU::_registers[(CPU::instr >> 3) & 3UL] = zero_Ext(CPU::data_memory.mem_read8(mapped.second));
        }
        else if (mapped.first == 2)
        {
            CPU::_registers[(CPU::instr >> 3) & 3UL] = zero_Ext(CPU::data_memory.mem_read16(mapped.second));
        }
        else if (mapped.first == 4)
        {
            CPU::_registers[(CPU::instr >> 3) & 3UL] = zero_Ext(CPU::data_memory.mem_read32(mapped.second));
        }
        else if (mapped.first == 8)
        {
            CPU::_registers[(CPU::instr >> 3) & 3UL] = zero_Ext(CPU::data_memory.mem_read64(mapped.second));
        }
        break;
    }
    }
}

void InstructionsImpl::movsx()
{
    // this is also exactly the same as movzx but it instead extends the sign of the values
    // this extends the values by 1's only
    short format = (CPU::instr >> 56) & 3UL; // get the format type
    switch (format)
    {
    case 0:
    {
        // for register-register mov: 00
        //  000000 00 00000000 00000000 00000000 00000000 00000000 00000000 00 000 000
        // last 6 bits for the operands
        CPU::_registers[(CPU::instr >> 3) & 7UL] = sign_Ext(CPU::_registers[CPU::instr & 7UL], 1); // put the value
        break;
    }
    case 1:
    {
        // for register-imm mov: 01
        //  000000 01 00000000 00000000 00000000 00000000 00000000 00000000 00000 000
        // last 3 bits for the destination register and the remaining bits for the immediate value
        // the immediate can be as large as 2^53 bits which is very large for an immediate
        CPU::_registers[(CPU::instr & 7UL)] = sign_Ext((CPU::instr >> 3) & 0x1FFFFFFFFFFFFF, 1); // get the immediate value
        break;
    }
    case 2:
    {
        // for register-addr mov: 10
        //  000000 10 00000000 00000000 00000000 00000000 00000000 00000000 00 000 000
        // last 6 bits are for the operand registers
        // this is the same as case 0, why? for clear distinction, to know that the value being handled is clearly an address
        // you can use 00 as well which does the same
        CPU::_registers[(CPU::instr >> 3) & 7UL] = sign_Ext(CPU::_registers[CPU::instr & 7UL], 1); // put the address
        break;
    }
    case 3:
    {
        // for register-register[addr] mov: 11
        //  000000 10 00000000 00000000 00000000 00000000 00000000 00000000 00 000 000
        // this operation moves the value at the address at the source register and saves it in the destination register
        // the address should be loaded into the register first
        auto mapped = map_mem(CPU::_registers[CPU::instr & 3UL]);
        if (mapped.first == 1)
        {
            CPU::_registers[(CPU::instr >> 3) & 3UL] = sign_Ext(CPU::data_memory.mem_read8(mapped.second), 1);
        }
        else if (mapped.first == 2)
        {
            CPU::_registers[(CPU::instr >> 3) & 3UL] = sign_Ext(CPU::data_memory.mem_read16(mapped.second), 1);
        }
        else if (mapped.first == 4)
        {
            CPU::_registers[(CPU::instr >> 3) & 3UL] = sign_Ext(CPU::data_memory.mem_read32(mapped.second), 1);
        }
        else if (mapped.first == 8)
        {
            CPU::_registers[(CPU::instr >> 3) & 3UL] = sign_Ext(CPU::data_memory.mem_read64(mapped.second), 1);
        }
        break;
    }
    }
}

void InstructionsImpl::store()
{
    // 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    // this instruction only takes address and the destination register,
    std::uint8_t regr = CPU::_registers[CPU::instr & 3UL];
    CPU::_registers[CPU::pc]++;
    CPU::fetch(); // address must be in the next address
    auto mapped = map_mem(CPU::instr);
    if (mapped.first == 1)
    {
        CPU::_registers[(regr)] = zero_Ext(CPU::data_memory.mem_read8(mapped.second));
    }
    else if (mapped.first == 2)
    {
        CPU::_registers[(regr)] = zero_Ext(CPU::data_memory.mem_read16(mapped.second));
    }
    else if (mapped.first == 4)
    {
        CPU::_registers[(regr)] = zero_Ext(CPU::data_memory.mem_read32(mapped.second));
    }
    else if (mapped.first == 8)
    {
        CPU::_registers[(regr)] = zero_Ext(CPU::data_memory.mem_read64(mapped.second));
    }
}

void InstructionsImpl::load()
{
    // 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    // this instruction only takes imm value and the destination register
    CPU::_registers[CPU::instr & 3UL] = (CPU::instr >> 3) & 0x3FFFFFFFFFFFFFF;
}

void InstructionsImpl::lea()
{
    // this is the same as store except, it stores the address instead of the value AND directly to ar
    CPU::_registers[CPU::pc]++;
    CPU::fetch(); // address must be in the next address
    CPU::_registers[CPU::ar] = CPU::instr;
}

void InstructionsImpl::push()
{
    // this instruction pushes all of the values in the registers to the stack in the order
    // ar, br, cr, dr, er1, er2, er3, er4
    for (int i = 0; i < 8; i++)
    {
        CPU::data_memory.mem_write64(CPU::_registers[CPU::sp], CPU::_registers[i]);
        CPU::_registers[CPU::sp] += 8;
    }
}

void InstructionsImpl::pop()
{
    // this instruction pops the values in the stack to the registers in the order
    // er4, er3, er2, er1, dr, cr, br, ar
    for (int i = 7; i >= 0; i--)
    {
        CPU::_registers[CPU::sp] -= 8;
        CPU::_registers[i] = CPU::data_memory.mem_read64(CPU::_registers[CPU::sp]);
    }
}

void InstructionsImpl::pushr()
{
    // push the specified register to the top of stack
    CPU::data_memory.mem_write64(CPU::_registers[CPU::sp], CPU::_registers[CPU::instr & 3UL]);
    CPU::_registers[CPU::sp] += 8;
}

void InstructionsImpl::popr()
{
    // pop the top to stack to the specified register
    CPU::_registers[CPU::sp] -= 8;
    CPU::_registers[CPU::instr & 3UL] = CPU::data_memory.mem_read64(CPU::_registers[CPU::sp]);
}

void InstructionsImpl::movz()
{
    if (CPU::flags[CPU::ZERO] == 1)
    {
        mov();
    }
}

void InstructionsImpl::movnz()
{
    if (CPU::flags[CPU::ZERO] != 1)
    {
        mov();
    }
}

void InstructionsImpl::move()
{
    if (CPU::flags[CPU::EQUAL] == 1)
    {
        mov();
    }
}

void InstructionsImpl::movne()
{
    if (CPU::flags[CPU::NOT_EQ] == 1)
    {
        mov();
    }
}

void InstructionsImpl::movg()
{
    if (CPU::flags[CPU::GREATER] == 1)
    {
        mov();
    }
}

void InstructionsImpl::movge()
{
    if (CPU::flags[CPU::GREATER_EQ] == 1)
    {
        mov();
    }
}

void InstructionsImpl::movs()
{
    if (CPU::flags[CPU::SMALLER] == 1)
    {
        mov();
    }
}

void InstructionsImpl::movse()
{
    if (CPU::flags[CPU::SMALLER] == 1)
    {
        mov();
    }
}

void InstructionsImpl::iand()
{
    // 000000 0 0 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    // can only apply and to register-register and register-imm
    short format = (CPU::instr >> 56) & 1UL;
    if (format == 1)
    {
        CPU::_registers[CPU::instr & 3UL] &= (CPU::instr >> 3) & 0x1FFFFFFFFFFFFF;
    }
    else
    {
        CPU::_registers[(CPU::instr >> 3) & 3UL] &= CPU::_registers[CPU::instr & 3UL];
    }
}

void InstructionsImpl::inot()
{
    //  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    // can only apply to registers
    auto pos = CPU::instr & 3UL;
    CPU::_registers[pos] = ~CPU::_registers[pos];
}

void InstructionsImpl::ior()
{
    //  000000 0 0 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    // can only or a register to another register or register to an immediate value
    short format = (CPU::instr >> 56) & 1UL;
    if (format == 1)
    {
        CPU::_registers[CPU::instr & 3UL] |= (CPU::instr >> 3) | 0x1FFFFFFFFFFFFF;
    }
    else
    {
        CPU::_registers[(CPU::instr >> 3) & 3UL] |= CPU::_registers[CPU::instr & 3UL];
    }
}

void InstructionsImpl::ixor()
{
    //  000000 0 0 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    //  can only xor a register to another register or register to an immediate value
    short format = (CPU::instr >> 56) & 1UL;
    if (format == 1)
    {
        CPU::_registers[CPU::instr & 3UL] ^= (CPU::instr >> 3) ^ 0x1FFFFFFFFFFFFF;
    }
    else
    {
        CPU::_registers[(CPU::instr >> 3) & 3UL] ^= CPU::_registers[CPU::instr & 3UL];
    }
}

void InstructionsImpl::lshift()
{
    //  00000000 0 0000000 00000000 00000000 00000000 00000000 00000000 00000000
    //  can only lshift the value in a register according to the number of bits in another register
    //  or according to the given immediate bits
    short format = (CPU::instr >> 56) & 1UL;
    if (format == 1)
    {
        CPU::_registers[CPU::instr & 3UL] = CPU::_registers[CPU::instr & 3UL] >> ((CPU::instr >> 3) ^ 0x1FFFFFFFFFFFFF);
    }
    else
    {
        CPU::_registers[(CPU::instr >> 3) & 3UL] = CPU::_registers[(CPU::instr >> 3) & 3UL] >> CPU::_registers[CPU::instr & 3UL];
    }
}

void InstructionsImpl::rshift()
{
    //  000000 0 0 00000000 00000000 00000000 00000000 00000000 00000000 00000 000
    //  can only lshift the value in a register according to the number of bits in another register
    //  or according to the given immediate bits
    short format = (CPU::instr >> 56) & 1UL;
    if (format == 1)
    {
        CPU::_registers[CPU::instr & 3UL] = CPU::_registers[CPU::instr & 3UL] << ((CPU::instr >> 3) ^ 0x1FFFFFFFFFFFFF);
    }
    else
    {
        CPU::_registers[(CPU::instr >> 3) & 3UL] = CPU::_registers[(CPU::instr >> 3) & 3UL] << CPU::_registers[CPU::instr & 3UL];
    }
}

void InstructionsImpl::add()
{
    // 000000 00 00000000 00000000 00000000 00000000 00000000 00000000 00000 000
    // add can be of 4 types: register-register, register-immediate, immediate-register, register-memory
    // register-immediate and immediate-register are the same with different representing bits
    auto format = CPU::instr >> 56;
    switch (format)
    {
    case 0:
    {
        // R-R
        CPU::_registers[(CPU::instr >> 3) & 3UL] += CPU::_registers[CPU::instr & 3UL];
        break;
    }
    case 1:
    case 2:
    {
        CPU::_registers[(CPU::instr & 3UL)] += (CPU::instr >> 3) & 0x1FFFFFFFFFFFFF;
        break;
    }
    case 3:
    {
        auto reg = CPU::instr & 3UL;
        CPU::_registers[CPU::pc]++;
        CPU::fetch();
        auto mapped = map_mem(CPU::instr);
        if (mapped.first == 1)
        {
            CPU::_registers[reg] += CPU::data_memory.mem_read8(mapped.second);
        }
        else if (mapped.first == 2)
        {
            CPU::_registers[reg] += CPU::data_memory.mem_read16(mapped.second);
        }
        else if (mapped.first == 4)
        {
            CPU::_registers[reg] += CPU::data_memory.mem_read32(mapped.second);
        }
        else if (mapped.first == 8)
        {
            CPU::_registers[reg] += CPU::data_memory.mem_read64(mapped.second);
        }
        break;
    }
    }
}

void InstructionsImpl::sub()
{
    auto format = CPU::instr >> 56;
    switch (format)
    {
    case 0:
    {
        // R-R
        CPU::_registers[(CPU::instr >> 3) & 3UL] -= CPU::_registers[CPU::instr & 3UL];
        break;
    }
    case 1:
    case 2:
    {
        CPU::_registers[(CPU::instr & 3UL)] -= (CPU::instr >> 3) & 0x1FFFFFFFFFFFFF;
        break;
    }
    case 3:
    {
        auto reg = CPU::instr & 3UL;
        CPU::_registers[CPU::pc]++;
        CPU::fetch();
        auto mapped = map_mem(CPU::instr);
        if (mapped.first == 1)
        {
            CPU::_registers[reg] -= CPU::data_memory.mem_read8(mapped.second);
        }
        else if (mapped.first == 2)
        {
            CPU::_registers[reg] -= CPU::data_memory.mem_read16(mapped.second);
        }
        else if (mapped.first == 4)
        {
            CPU::_registers[reg] -= CPU::data_memory.mem_read32(mapped.second);
        }
        else if (mapped.first == 8)
        {
            CPU::_registers[reg] -= CPU::data_memory.mem_read64(mapped.second);
        }
        break;
    }
    }
}


void InstructionsImpl::mul()
{
    auto format = CPU::instr >> 56;
    switch (format)
    {
    case 0:
    {
        // R-R
        CPU::_registers[(CPU::instr >> 3) & 3UL] *= CPU::_registers[CPU::instr & 3UL];
        break;
    }
    case 1:
    case 2:
    {
        CPU::_registers[(CPU::instr & 3UL)] *= (CPU::instr >> 3) & 0x1FFFFFFFFFFFFF;
        break;
    }
    case 3:
    {
        auto reg = CPU::instr & 3UL;
        CPU::_registers[CPU::pc]++;
        CPU::fetch();
        auto mapped = map_mem(CPU::instr);
        if (mapped.first == 1)
        {
            CPU::_registers[reg] *= CPU::data_memory.mem_read8(mapped.second);
        }
        else if (mapped.first == 2)
        {
            CPU::_registers[reg] *= CPU::data_memory.mem_read16(mapped.second);
        }
        else if (mapped.first == 4)
        {
            CPU::_registers[reg] *= CPU::data_memory.mem_read32(mapped.second);
        }
        else if (mapped.first == 8)
        {
            CPU::_registers[reg] *= CPU::data_memory.mem_read64(mapped.second);
        }
        break;
    }
    }
}


void InstructionsImpl::div()
{
    auto format = CPU::instr >> 56;
    switch (format)
    {
    case 0:
    {
        // R-R
        CPU::_registers[(CPU::instr >> 3) & 3UL] /= CPU::_registers[CPU::instr & 3UL];
        break;
    }
    case 1:
    case 2:
    {
        CPU::_registers[(CPU::instr & 3UL)] /= (CPU::instr >> 3) & 0x1FFFFFFFFFFFFF;
        break;
    }
    case 3:
    {
        auto reg = CPU::instr & 3UL;
        CPU::_registers[CPU::pc]++;
        CPU::fetch();
        auto mapped = map_mem(CPU::instr);
        if (mapped.first == 1)
        {
            CPU::_registers[reg] /= CPU::data_memory.mem_read8(mapped.second);
        }
        else if (mapped.first == 2)
        {
            CPU::_registers[reg] /= CPU::data_memory.mem_read16(mapped.second);
        }
        else if (mapped.first == 4)
        {
            CPU::_registers[reg] /= CPU::data_memory.mem_read32(mapped.second);
        }
        else if (mapped.first == 8)
        {
            CPU::_registers[reg] /= CPU::data_memory.mem_read64(mapped.second);
        }
        break;
    }
    }
}

void instructionsImpl::inc()
{
  //  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
// takes a register as thr only operand and increments it
CPU::_registers[CPU::instr & 3UL]++;
}

void instructionsImpl::dec()
{
  //  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
// takes a register as thr only operand and increments it
CPU::_registers[CPU::instr & 3UL]--;
}

void InstructionsImpl::neg()
{
//this just takes a register as operand and applies two's complement on its value
CPU::_registers[CPU::instr & 3UL] = twoComplement(CPU::_registers[CPU::instr & 3UL]);
}

#endif
//  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000

#ifndef ENIGMA_CPU
#define ENIGMA_CPU

#include "../memory/EnigmaMemory.hpp"
#ifndef ENIGMA_MANAGER
#include "../Manager/EnigmaManager.hpp"
#endif
#define STACK_START 0x0
#define STACK_END 0xFF

#define DATA_MEM_START 0x100;

static std::uint64_t start_reading_instrs = DATA_MEM_START;

static std::uint64_t sign_Ext(std::uint64_t toext, int ext_bit)
{
    if (ext_bit == 1)
    {
        return toext & 0b1111111111111111111111111111111111111111111111111111111111111111;
    }
    return toext;
}

static std::uint64_t zero_Ext(std::uint64_t toext)
{
    return toext | 0UL;
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

namespace CPU
{
    static Memory instruction_memory;
    static Memory data_memory;

    static std::uint64_t mem_pointer = DATA_MEM_START;

    static bool running = true;

    enum Register : qword
    {
        ar,
        br,
        cr,
        dr,
        er1,
        er2,
        er3,
        er4,
        sp,
        pc,
        regr_count
    };

    enum Flags : byte
    {
        ZERO,
        NONZERO,
        GREATER,
        SMALLER,
        EQUAL,
        NOT_EQ,
        GREATER_EQ,
        SMALLER_EQ,
        FLAGS_COUNT
    };

    enum Instructions
    {
        NOP, // the no operation instruction which means don't do anything
        // basic arithmetic operations
        ADD,
        SUB,
        MUL,
        DIV,
        INC,
        DEC,
        NEG, // make the value at specified register negative by applying 2's complement

        // logical operations
        AND,
        NOT,
        OR,
        XOR,
        LSHIFT, // shift the value left at the specified register by the specified bits
        RSHIFT, // shift the value right at the specified register by the specified bits

        // register based operations
        MOV,
        MOVZX,    // zero-extend and move making it 64 bit
        MOVSX,    // sign extend and move making it 64 bit but sign extended
        STORE,    // store address or immediates
        LOAD,     // load value from given address
        LEA,      // load the given address to the given register
        PUSH,     // push all of the values in every register in a specific order to the stack
        POP,      // pop all of the values to every register in a specific order from the stack
        PUSH_REG, // push specific register to the stack
        POP_REG,  // pop to specific register from the stack

        // conditional jumps
        CMP,
        JMP,
        JZ,
        JNZ,
        JN,  // jmp if negative
        JNN, // jmp if not negative
        JE,
        JNE,
        JG,
        JGE,
        JS,
        JSE,

        // conditional operations for regsiters
        MOVZ,
        MOVNZ,
        MOVN,
        MOVNN,
        MOVE,
        MOVNE,
        MOVG,
        MOVGE,
        MOVS,
        MOVSE,

        // call is equivalent to jmp and this will be handled by the assembler generating the code

        HALT,
        SYSCALL,
    };

    // 6 bits will be dedicated to instructions since it implies for a possiblility of 63 instructions and we
    // currently have 50 leaving 13 for expansion

    static qword _registers[regr_count];
    static byte flags[FLAGS_COUNT];
    static qword instr;
    static byte curr_instr;

    // this function initializes the stack pointer and instruction pointer to 0. stack begins from 0 to 255 and then it can be whatever it want
    inline void init()
    {
        _registers[sp] = STACK_START;
        _registers[pc] = 0x0;
    }

    inline void fetch()
    {
        instr = instruction_memory.mem_read64(_registers[pc]);
    }

    inline void decode()
    {
        curr_instr = (instr >> 58);
    }

#include "EnigmaInstructions.hpp"

    inline void execute()
    {
        switch (curr_instr)
        {
        case ADD:
            InstructionsImpl::add();
            break;
        case SUB:
            InstructionsImpl::sub ();
            break;
        case MUL:
            InstructionsImpl::mul();
            break;
        case DIV:
            InstructionsImpl::div();
            break;
        case INC:
            InstructionsImpl::inc();
            break;
        case DEC:
            InstructionsImpl::dec();
            break;
        case NEG:
            InstructionsImpl::neg();
            break;
        case MOV:
            InstructionsImpl::mov();
            break;
        case MOVSX:
            InstructionsImpl::movsx();
            break;
        case MOVZX:
            InstructionsImpl::movzx();
            break;
        case LOAD:
            InstructionsImpl::load();
            break;
        case STORE:
            InstructionsImpl::store();
            break;
        case LEA:
            InstructionsImpl::lea();
            break;
        case PUSH:
            InstructionsImpl::push();
            break;
        case POP:
            InstructionsImpl::pop();
            break;
        case PUSH_REGR:
            InstructionsImpl::pushr();
            break;
        case POP_REGR:
            InstructionsImpl::popr();
            break;
        case AND:
            InstructionsImpl::iand();
            break;
        case OR:
            InstructionsImpl::ior();
            break;
        case NOT:
            InstructionsImpl::inot();
            break;
        case XOR:
            InstructionsImpl::ixor();
            break;
        case LSHIFT:
            InstructionsImpl::lshift();
            break;
        case RSHIFT:
            InstructionsImpl::rshift();
            break;
        case CMP:
            InstructionsImpl::cmp();
            break;
        case JMP:
            InstructionsImpl::jmp();
            break;
        case JE:
            InstructionsImpl::je();
            break;
        case JG:
            InstructionsImpl::jg();
            break;
        case JGE:
            InstructionsImpl::jge();
            break;
        case JNE:
            InstructionsImpl::jne();
            break;
        case JNZ:
            InstructionsImpl::jnz();
            break;
        case JS:
            InstructionsImpl::js();
            break;
        case JSE:
            InstructionsImpl::jse();
            break;
        case JZ:
            InstructionsImpl::jz();
            break;
        case MOVZ:
            InstructionsImpl::movz();
            break;
        case MOVNZ:
            InstructionsImpl::movnz();
            break;
        case MOVN:
            InstructionsImpl::movn();
            break;
        case MOVNN:
            InstructionsImpl::movnn();
            break;
        case MOVE:
            InstructionsImpl::move();
            break;
        case MOVNE:
            InstructionsImpl::movne();
            break;
        case MOVG:
            InstructionsImpl::movg();
            break;       
        case MOVGE:
            InstructionsImpl::movge();
            break;
        case MOVS:
            InstructionsImpl::movs();
            break;
        case MOVSE:
            InstructionsImpl::movse();
            break;       
        case NOP:
            break;
        case SYSCALL:
            Manager::handlesyscalls();
            break;
        case HALT:
            running = false;
            break;
        }
    }

    inline void run()
    {
        while (running == true)
        {
            fetch();
            decode();
            execute();
            _registers[pc]++;
        }
    }
};

#endif
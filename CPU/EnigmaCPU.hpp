#ifndef ENIGMA_CPU
#define ENIGMA_CPU

#include "../memory/EnigmaMemory.hpp"
#ifndef ENIGMA_MANAGER
#include "../Manager/EnigmaManager.hpp"
#endif
#define STACK_START 0x0
#define STACK_END 0xFF

#define DATA_MEM_START  0x100;

static std::uint64_t start_reading_instrs = DATA_MEM_START;

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
        NEGATIVE,
        NON_NEGATIVE,
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
        NOP, //the no operation instruction which means don't do anything
        //basic arithmetic operations
        ADD,
        SUB,
        MUL,
        DIV,
        INC,
        DEC,
        NEG, //make the value at specified register negative by applying 2's complement

        //logical operations
        AND,
        NOT,
        OR,
        XOR,
        LSHIFT, //shift the value left at the specified register by the specified bits
        RSHIFT, //shift the value right at the specified register by the specified bits

        //register based operations
        MOV,
        MOVZX, //zero-extend and move making it 64 bit
        MOVSX, //sign extend and move making it 64 bit but sign extended
        STORE, //store address or immediates
        LOAD,  //load value from given address
        LEA,   // load the given address to the given register
        PUSH,  //push all of the values in every register in a specific order to the stack
        POP,   //pop all of the values to every register in a specific order from the stack
        PUSH_REG, //push specific register to the stack
        POP_REG,  //pop to specific register from the stack

        //conditional jumps
        CMP,
        JMP,
        JZ,
        JNZ,
        JN, //jmp if negative
        JNN,//jmp if not negative
        JE,
        JNE,
        JG,
        JGE,
        JS,
        JSE,

        //conditional operations for regsiters
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

        //extensions
        EXT, //sign extend the value in specified register
        ZEXT, //zero extend the value in specified register

        //call is equivalent to jmp and this will be handled by the assembler generating the code

        HALT,
        SYSCALL,
    };

    //6 bits will be dedicated to instructions since it implies for a possiblility of 63 instructions and we
    //currently have 50 leaving 13 for expansion

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
#ifndef ISA_H
#define ISA_H

#include <string>

enum REGISTER{
    zero=0,
    a,
    b,
    c,
    d,
    sp=0x10,
    ra=0x11,
    pc=0x1F,
};

enum OPCODE{
    NOP,
    MOV,
    LDI,
    LDR,
    STR,
    PUSH,
    POP,

    ADD,
    SUB,
    MUL,
    DIV,

    ADDI,
    SUBI,

    AND,
    OR,
    XOR,
    NOT,
    SLL,
    SAL,
    SHR,

    CMP,
    SLT,
    BNE,
    BEQ,
    BZ,
    BNZ,

    JMP,
    CALL,
    RET,
    EXIT,
};

union Data{
    struct instruction{
        unsigned int immediate:32-15;
        REGISTER dest:5;
        REGISTER source:5;
        OPCODE opcode:5;
    }instruction;
    unsigned int integer;
};

OPCODE getOPCode(std::string s);

#endif

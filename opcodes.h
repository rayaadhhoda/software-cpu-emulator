#include <string>
#ifndef OPCODE_H
#define OPCODE_H
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

OPCODE getOPCode(std::string s);
#endif
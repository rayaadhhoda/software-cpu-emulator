#include "isa.h"

#include <iostream>
#include <unordered_map>

OPCODE getOPCode(std::string s){
    static std::unordered_map<std::string,OPCODE> const opcodes={
        {"NOP",NOP},
        {"MOV",MOV},
        {"LDI",LDI},
        {"LDR",LDR},
        {"STR",STR},
        {"PUSH",PUSH},
        {"POP",POP},
        {"ADD",ADD},
        {"SUB",SUB},
        {"MUL",MUL},
        {"DIV",DIV},
        {"ADDI",ADDI},
        {"SUBI",SUBI},
        {"AND",AND},
        {"OR", OR},
        {"XOR",XOR},
        {"NOT",NOT},
        {"SLL",SLL},
        {"SAL",SAL},
        {"SHR",SHR},
        {"CMP",CMP},

        {"SLT",SLT},
        {"BNE",BNE},
        {"BEQ",BEQ},
        {"BZ",BZ},
        {"BNZ",BNZ},

        {"JMP",JMP},
        {"CALL",CALL},
        {"EXIT",EXIT},
        {"RET",RET},
    };
    auto result = opcodes.find(s);
    if (result == opcodes.end()) {
        std::cout<<"invalid opcode!"<< s <<std::endl;
        return NOP;
    }
    return result->second;
}

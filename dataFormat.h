#ifndef DATAFORMAT_H
#define DATAFORMAT_H
#include "opcodes.h"
#include "register.h"
union Data{
    struct instruction{
        unsigned int immediate:32-15;
        REGISTER dest:5;
        REGISTER source:5;
        OPCODE opcode:5;
    }instruction;
    unsigned int integer;
    char character;
};
#endif
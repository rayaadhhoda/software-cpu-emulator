#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "isa.h"

#include <string>
#include <unordered_map>

enum STATE{
    GLOBAL,
    TEXT,
    NONE,
};

Data encodeInstruction(std::string line, std::unordered_map<std::string, int>& labelLineNum);
void populateLabels(std::unordered_map<std::string, int>& labelLineNum);

#endif

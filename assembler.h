

#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <string>
#include <unordered_map>
#include "register.h"
#include "opcodes.h"
#include "dataFormat.h"


int assemble(std::string filename,Data *instructions,int size,std::unordered_map<std::string, int> &labelLineNum);

#endif
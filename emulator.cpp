#include <iostream>
#include <bitset>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include "opcodes.h"
#include "assembler.h"
#include "simulate.h"

int main(){

    const int INSTR_MEM_SIZE=1024;
    std::unordered_map<std::string, int> labelLineNum;
    Data instructions[INSTR_MEM_SIZE];
    int lineNum = assemble("fibonacci.asm",instructions,INSTR_MEM_SIZE,labelLineNum);

    simulate(instructions,lineNum);
    return 0;    
    
}

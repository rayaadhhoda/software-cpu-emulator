#include <iostream>
#include <bitset>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include "opcodes.h"
#include "assembler.h"
#include "simulate.h"

int main(int argc, char*argv[]){

    const int INSTR_MEM_SIZE=1024;
    std::unordered_map<std::string, int> labelLineNum;
    Data instructions[INSTR_MEM_SIZE];

    if(argc<2){
        std::cout<<"please specify a filename"<<std::endl;
        return 0;
    }
    
    //avengers...
    int lineNum = assemble(argv[1],instructions,INSTR_MEM_SIZE,labelLineNum);

    simulate(instructions,lineNum);
    return 0;
}

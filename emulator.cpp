#include <iostream>
#include <bitset>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include "opcodes.h"
#include "assembler.h"
#include "simulate.h"
#include <fstream>

int main(int argc, char*argv[]){

    const int INSTR_MEM_SIZE=1024;
    std::unordered_map<std::string, int> labelLineNum;
    Data instructions[INSTR_MEM_SIZE];

    if(argc<2){
        std::cout<<"please specify a filename"<<std::endl;
        return 0;
    }
    bool verbose=false;
    if(argc==3){
        verbose=true;
    }
    
    //avengers...
    int lineNum = assemble(argv[1],instructions,INSTR_MEM_SIZE,labelLineNum);


    std::ofstream out("assembled.txt");

    for (int i = 0; i < lineNum; i++) {
        //i love c++ i love c++ i love c++
        out << std::bitset<32>(instructions[i].integer) << '\n';
    }

    out.close();

    simulate(instructions,lineNum,verbose);
    return 0;
}

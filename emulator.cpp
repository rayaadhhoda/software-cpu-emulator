#include "assembler.h"
#include "cpu.h"

#include <fstream>
#include <unordered_map>
#include <sstream>

int main(){
    /*
    data i;
    i.integer=0;
    i.instruction.immediate=__INT_MAX__;
    std::cout<<std::bitset<32>(i.integer)<<std::endl;
    */

    std::unordered_map<std::string, int> labelLineNum;
    populateLabels(labelLineNum);
    std::string word, line;
    std::ifstream assemblyInput("fibonacci.asm");
    STATE state;
    Data instructions[1024];
    int lineNum=0;
    for(auto &e:instructions) e.integer=0;

    while(std::getline(assemblyInput,line)){
        std::stringstream ssline(line);
        if(!(ssline>>word)){
            continue;
        }
        if(word==".global") {
            state=GLOBAL;
            continue;
        }
        else if(word==".text") {
            state=TEXT;
            continue;
        }

        switch(state){
            case TEXT:
                if(*(word.end()-1) == ':'){
                    break;
                }
                instructions[lineNum]=encodeInstruction(line,labelLineNum);
                lineNum++;
                break;
            case GLOBAL:
                break;
            case NONE:
                break;
            default:
                //This should not happen
                break;
        }        
    }
    /*
    for(auto e:labelLineNum){
        std::cout<<e.first<<", "<<e.second<<std::endl;
        //std::cout<<std::bitset<32>(instructions[e.second].integer)<<std::endl;
        std::cout<<instructions[e.second].instruction.opcode<<std::endl;
    }
    */
    /*
    std::cout<<"HERE ARE THE INSTRUCTIONS LINECOUNT: "<< lineNum <<std::endl;
    for(int i=0;i<32;i++){
        for(auto e:labelLineNum){
            if(e.second==i)std::cout<<e.first;
        }
        std::cout<<"line "<< i << ": "<< instructions[i].instruction.opcode<<std::endl;
    }
    */
    

    simulate(instructions,lineNum);
    return 0;    
    
}

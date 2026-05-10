#include "assembler.h"
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>



enum STATE{
    GLOBAL,
    TEXT,
    NONE,
};

Data encodeInstruction(std::string line, std::unordered_map<std::string, int>& labelLineNum){
    std::stringstream ssline(line);
    std::string word;

    ssline>>word;
    Data instr;
    instr.integer=0;

    instr.instruction.opcode = getOPCode(word);
    bool dest=false, source=false;
    while(ssline>>word){
        word.erase(std::remove(word.begin(),word.end(),','), word.end());
        if(word[0]=='#' || word[0]=='['){
            std::stringstream imm(word);
            char discard;
            int num;
            imm>>discard>>num;
            instr.instruction.immediate=num;
        }
        else if (labelLineNum.find(word+':')!=labelLineNum.end()){
            instr.instruction.immediate = labelLineNum[word+':'];
        }
        else if(word[0]=='%'){
            REGISTER reg=zero;
            if(word.compare("%a")==0){
                reg = a;
            }
            else if(word.compare("%b")==0){
                reg=b;
            }
            else if(word.compare("%ra")==0){
                reg=ra;
            }
            else if(word.compare("%sp")==0){
                reg=sp;
            }
            
            if(!dest){
                instr.instruction.dest=reg;
                dest=true;
            }
            else if(!source){
                instr.instruction.source=reg;
                source=true;
            }
            else{
                std::cout<<"TOO MANY ARGS"<<std::endl;
            }
        }
    }
    return instr;
}

void populateLabels(std::unordered_map<std::string, int>& labelLineNum){
    std::string word, line;
    std::ifstream assemblyInput("fibonacci.asm");
    STATE state;
    int lineNum=0;

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
                //associate label with instruction number (technically not very robust since variable names should also be treated as labels, but we're not worried about that rn)
                if(*(word.end()-1) == ':'){
                    labelLineNum[word]=lineNum;
                    break;
                }
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
    assemblyInput.close();
    return;
}

int assemble(std::string filename,Data *instructions,int size,std::unordered_map<std::string, int> &labelLineNum){
    populateLabels(labelLineNum);
    std::string word, line;
    std::ifstream assemblyInput("fibonacci.asm");
    STATE state;
    
    int lineNum=0;
    for(int i=0;i<size;i++){
        instructions[i].integer=0;
    }

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
    return lineNum;
}


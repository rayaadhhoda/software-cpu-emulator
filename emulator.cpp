#include <iostream>
#include <bitset>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <sstream>

union Data{
    struct instruction{
        unsigned int immediate:32-15;
        unsigned int dest:5;
        unsigned int source:5;
        unsigned int opcode:5;
    }instruction;
    unsigned int integer;
};

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

    AND,
    OR,
    XOR,
    NOT,
    SLL,
    SAL,
    SHR,

    CMP,
    JMP,
    JAL,
    RET,
};

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
        {"AND",AND},
        {"OR", OR},
        {"XOR",XOR},
        {"NOT",NOT},
        {"SLL",SLL},
        {"SAL",SAL},
        {"SHR",SHR},
        {"CMP",CMP},
        {"JMP",JMP},
        {"JAL",JAL},
        {"RET",RET},
    };
    auto result = opcodes.find(s);
    if (result == opcodes.end()) {
        std::cout<<"invalid opcode!" <<std::endl; 
        return NOP;
    }
    return result->second;
}

enum STATE{
    GLOBAL,
    TEXT,
    NONE,
};

Data encodeInstruction(std::string line){
    std::stringstream ssline(line);
    std::string word;

    ssline>>word;
    Data instr;
    instr.integer=0;

    instr.instruction.opcode = getOPCode(word);
    return instr;
}

int main(){
    /*
    data i;
    i.integer=0;
    i.instruction.immediate=__INT_MAX__;
    std::cout<<std::bitset<32>(i.integer)<<std::endl;
    */

    std::unordered_map<std::string, int> labelLineNum;
    std::string word, line;
    std::ifstream assemblyInput("fibonacci.asm");
    STATE state;
    Data instructions[1024];
    int lineNum=0;
    for(auto &e:instructions) e.integer=0;

    while(std::getline(assemblyInput,line)){
        std::stringstream ssline(line);
        ssline>>word;
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
                instructions[lineNum]=encodeInstruction(line);
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

    for(auto e:labelLineNum){
        std::cout<<e.first<<", "<<e.second<<std::endl;
    }
    
}

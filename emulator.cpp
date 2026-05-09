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

enum REGISTER{
    zero=0,
    a,
    b,
    c,
    d,
    sp=0x10,
    ra=0x11,
    pc=0x1F,
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
    SLT,
    BNE,
    BEQ,

    JMP,
    CALL,
    RET,
    EXIT,
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

        {"SLT",SLT},
        {"BNE",BNE},
        {"BEQ",BEQ},

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
        if(word[0]=='#'){
            instr.instruction.immediate=atoi(word.c_str()+1);
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
                std::cout<<word<<std::endl;
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

    for(auto e:labelLineNum){
        std::cout<<e.first<<", "<<e.second<<std::endl;
        //std::cout<<std::bitset<32>(instructions[e.second].integer)<<std::endl;
        std::cout<<instructions[e.second].instruction.opcode<<std::endl;
    }

    std::cout<<"HERE ARE THE INSTRUCTIONS LINECOUNT: "<< lineNum <<std::endl;
    for(int i=0;i<32;i++){
        for(auto e:labelLineNum){
            if(e.second==i)std::cout<<e.first;
        }
        std::cout<<"line "<< i << ": "<< instructions[i].instruction.dest<<std::endl;
        
    }

        
    
}

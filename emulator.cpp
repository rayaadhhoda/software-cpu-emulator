#include <iostream>
#include <bitset>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <sstream>



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
    SUBI,

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
    BZ,
    BNZ,

    JMP,
    CALL,
    RET,
    EXIT,
};

union Data{
    struct instruction{
        unsigned int immediate:32-15;
        REGISTER dest:5;
        REGISTER source:5;
        OPCODE opcode:5;
    }instruction;
    unsigned int integer;
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
        {"SUBI",SUBI},
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
        {"BZ",BZ},
        {"BNZ",BNZ},

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

// void populateLabels(std::unordered_map<std::string, int>& labelLineNum){
//     std::string word, line;
//     std::ifstream assemblyInput("fibonacci.asm");
//     STATE state;
//     int lineNum=0;

//     while(std::getline(assemblyInput,line)){
//         std::stringstream ssline(line);
//         if(!(ssline>>word)){
//             continue;
//         }
//         if(word==".global") {
//             state=GLOBAL;
//             continue;
//         }
//         else if(word==".text") {
//             state=TEXT;
//             continue;
//         }

//         switch(state){
//             case TEXT:
//                 //associate label with instruction number (technically not very robust since variable names should also be treated as labels, but we're not worried about that rn)
//                 if(*(word.end()-1) == ':'){
//                     labelLineNum[word]=lineNum;
//                     break;
//                 }
//                 lineNum++;
//                 break;
//             case GLOBAL:
//                 break;
//             case NONE:
//                 break;
//             default:
//                 //This should not happen
//                 break;
//         }        
//     }
//     assemblyInput.close();
//     return;
// }
void populateLabels(std::unordered_map<std::string, int>& labelLineNum, const std::string& filename){
    std::string word, line;
    std::ifstream assemblyInput(filename);
    STATE state = NONE;
    int lineNum = 0;

    while(std::getline(assemblyInput,line)){
        // remove comments
        auto commentPos = line.find(';');
        if(commentPos != std::string::npos){
            line = line.substr(0, commentPos);
        }

        std::stringstream ssline(line);
        if(!(ssline >> word)){
            continue;
        }

        if(word == ".global") {
            state = GLOBAL;
            continue;
        }
        else if(word == ".text") {
            state = TEXT;
            continue;
        }

        switch(state){
            case TEXT:
                if(*(word.end()-1) == ':'){
                    labelLineNum[word] = lineNum;
                    break;
                }
                lineNum++;
                break;
            case GLOBAL:
            case NONE:
                break;
        }
    }

    assemblyInput.close();
}

void simulate(Data *instructions, int numInstr){
    /*
    std::cout<<"HERE ARE THE INSTRUCTIONS LINECOUNT: "<< numInstr <<std::endl;
    for(int i=0;i<numInstr;i++){
        std::cout<<"line "<< i << ": "<< instructions[i].instruction.opcode<<std::endl;
    }
    */
    Data mem[1024];
    for(int i=0;i<1024;i++){
        mem[i].integer=0;
    }
    class CPU{
    public:
        Data gpReg[32];
        int sp, ra, pc, flag;
        CPU(){
            sp=0;
            ra=0;
            pc=0;
            flag=0;
            for(int i=0;i<32;i++){
                gpReg[i].integer=0;
            }
            return;
        }
        void nextInstruction(){
            pc++;
        }
    }cpu;
    std::string discard;
    while(true){
        //fetch
        Data currentInstruction = instructions[cpu.pc];
        cpu.pc++;
        //std::cout<<"opcode: "<< currentInstruction.instruction.opcode<<std::endl;
        //switch decodes opcode and then executes inside case
        switch(currentInstruction.instruction.opcode){
            case ADD:
                cpu.gpReg[currentInstruction.instruction.dest].integer+=cpu.gpReg[currentInstruction.instruction.source].integer;
                break;
            
            case LDI:
                // std::cout<<"loading immediate "<<currentInstruction.instruction.immediate<<std::endl;
                cpu.gpReg[currentInstruction.instruction.dest].integer=currentInstruction.instruction.immediate;
                //std::cin>>discard;
                break;
            case ADDI:
                cpu.gpReg[currentInstruction.instruction.dest].integer+=currentInstruction.instruction.immediate;
                break;
            case SUBI:
                cpu.gpReg[currentInstruction.instruction.dest].integer-=currentInstruction.instruction.immediate;
                break;
            case MOV:
                cpu.gpReg[currentInstruction.instruction.dest].integer=cpu.gpReg[currentInstruction.instruction.source].integer;
                break;
            case PUSH:
                if(currentInstruction.instruction.dest==REGISTER::ra){
                    mem[cpu.sp].integer = cpu.ra;
                }
                else{
                    mem[cpu.sp] = cpu.gpReg[currentInstruction.instruction.dest];
                }
                
                cpu.sp++;
                break;
            case POP:
                cpu.sp--;
                if(currentInstruction.instruction.dest==REGISTER::ra){
                    cpu.ra = mem[cpu.sp].integer;
                }
                else{
                    cpu.gpReg[currentInstruction.instruction.dest] = mem[cpu.sp];
                }                
                break;

            case STR: {
                int addr =
                    cpu.gpReg[currentInstruction.instruction.source].integer
                    + currentInstruction.instruction.immediate;

                int value =
                    cpu.gpReg[currentInstruction.instruction.dest].integer;

                if(addr == 12288){
                    std::cout << static_cast<char>(value);
                }
                else if(addr == 12289){
                    std::cout << value << std::endl;
                }

                break;
            }

            case EXIT:
                return;
                break;

            case SLT:
                if(cpu.gpReg[currentInstruction.instruction.source].integer < currentInstruction.instruction.immediate){
                    cpu.gpReg[currentInstruction.instruction.dest].integer=1;
                }
                else{
                    cpu.gpReg[currentInstruction.instruction.dest].integer=0;
                }
                break;
            
            case BZ:
                if(cpu.gpReg[currentInstruction.instruction.dest].integer==0){
                    cpu.pc=currentInstruction.instruction.immediate;
                }
                break;
            case BNZ:
                if(cpu.gpReg[currentInstruction.instruction.dest].integer!=0){
                    cpu.pc=currentInstruction.instruction.immediate;
                }
                break;
            case CALL:
                cpu.ra = cpu.pc;
                /*
                std::cout<<"CALL JUMP TO INSTRUCTION: "<<currentInstruction.instruction.immediate 
                << " a is: " << cpu.gpReg[REGISTER::a].integer 
                << "ra is: " <<cpu.ra
                <<std::endl;
                */
                cpu.pc = currentInstruction.instruction.immediate;
                break;
            case RET:
                cpu.pc = cpu.ra;
                //std::cout<<"RETURNING TO: "<<cpu.pc<<std::endl;
                break;
            default:
                std::cout<<"UNIMPLEMENTED INSTRUCTION IN CPU OPCODE: " << currentInstruction.instruction.opcode <<std::endl;
                return;
                break;
        }
    }
    return;
}

// int main(){
//     /*
//     data i;
//     i.integer=0;
//     i.instruction.immediate=__INT_MAX__;
//     std::cout<<std::bitset<32>(i.integer)<<std::endl;
//     */

//     std::unordered_map<std::string, int> labelLineNum;
//     populateLabels(labelLineNum);
//     std::string word, line;
//     std::ifstream assemblyInput("hello.asm");
//     STATE state;
//     Data instructions[1024];
//     int lineNum=0;
//     for(auto &e:instructions) e.integer=0;

//     while(std::getline(assemblyInput,line)){
//         std::stringstream ssline(line);
//         if(!(ssline>>word)){
//             continue;
//         }
//         if(word==".global") {
//             state=GLOBAL;
//             continue;
//         }
//         else if(word==".text") {
//             state=TEXT;
//             continue;
//         }

//         switch(state){
//             case TEXT:
//                 if(*(word.end()-1) == ':'){
//                     break;
//                 }
//                 instructions[lineNum]=encodeInstruction(line,labelLineNum);
//                 lineNum++;
//                 break;
//             case GLOBAL:
//                 break;
//             case NONE:
//                 break;
//             default:
//                 //This should not happen
//                 break;
//         }        
//     }
//     /*
//     for(auto e:labelLineNum){
//         std::cout<<e.first<<", "<<e.second<<std::endl;
//         //std::cout<<std::bitset<32>(instructions[e.second].integer)<<std::endl;
//         std::cout<<instructions[e.second].instruction.opcode<<std::endl;
//     }
//     */
//     /*
//     std::cout<<"HERE ARE THE INSTRUCTIONS LINECOUNT: "<< lineNum <<std::endl;
//     for(int i=0;i<32;i++){
//         for(auto e:labelLineNum){
//             if(e.second==i)std::cout<<e.first;
//         }
//         std::cout<<"line "<< i << ": "<< instructions[i].instruction.opcode<<std::endl;
//     }
//     */
    

//     simulate(instructions,lineNum);
//     return 0;    
    
// }
int main(int argc, char* argv[]){
    if(argc < 2){
        std::cout << "Usage: ./emulator <program.asm>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    std::unordered_map<std::string, int> labelLineNum;
    populateLabels(labelLineNum, filename);

    std::string word, line;
    std::ifstream assemblyInput(filename);

    if(!assemblyInput.is_open()){
        std::cout << "Could not open file: " << filename << std::endl;
        return 1;
    }

    STATE state = NONE;
    Data instructions[1024];
    int lineNum = 0;

    for(auto &e : instructions){
        e.integer = 0;
    }

    while(std::getline(assemblyInput,line)){
        // remove comments
        auto commentPos = line.find(';');
        if(commentPos != std::string::npos){
            line = line.substr(0, commentPos);
        }

        std::stringstream ssline(line);
        if(!(ssline >> word)){
            continue;
        }

        if(word == ".global") {
            state = GLOBAL;
            continue;
        }
        else if(word == ".text") {
            state = TEXT;
            continue;
        }

        switch(state){
            case TEXT:
                if(*(word.end()-1) == ':'){
                    break;
                }

                instructions[lineNum] = encodeInstruction(line, labelLineNum);
                lineNum++;
                break;

            case GLOBAL:
            case NONE:
                break;
        }
    }

    assemblyInput.close();

    simulate(instructions, lineNum);

    return 0;
}
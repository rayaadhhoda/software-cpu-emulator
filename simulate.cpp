#include "simulate.h"
#include "assembler.h"
#include <iostream>

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

            case STR:
                //store to this address to output a register as an unsigned immediate
                if(cpu.gpReg[currentInstruction.instruction.source].integer+currentInstruction.instruction.immediate==12288){
                    std::cout<<cpu.gpReg[currentInstruction.instruction.dest].integer<<std::endl;
                }
                //TODO: add character output
                //TODO: add memory write for variables
                break;

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
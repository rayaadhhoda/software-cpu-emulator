#include "isa.h"

#include <iomanip>
#include <sstream>
#include <unordered_map>

namespace softcpu {

std::string opcodeName(Opcode opcode) {
    switch (opcode) {
        case Opcode::NOP: return "NOP";
        case Opcode::MOV: return "MOV";
        case Opcode::LDI: return "LDI";
        case Opcode::LDR: return "LDR";
        case Opcode::STR: return "STR";
        case Opcode::ADD: return "ADD";
        case Opcode::SUB: return "SUB";
        case Opcode::INC: return "INC";
        case Opcode::DEC: return "DEC";
        case Opcode::AND: return "AND";
        case Opcode::OR: return "OR";
        case Opcode::XOR: return "XOR";
        case Opcode::NOT: return "NOT";
        case Opcode::SHL: return "SHL";
        case Opcode::SHR: return "SHR";
        case Opcode::CMP: return "CMP";
        case Opcode::JMP: return "JMP";
        case Opcode::JZ: return "JZ";
        case Opcode::JNZ: return "JNZ";
        case Opcode::JC: return "JC";
        case Opcode::JNC: return "JNC";
        case Opcode::PUSH: return "PUSH";
        case Opcode::POP: return "POP";
        case Opcode::CALL: return "CALL";
        case Opcode::RET: return "RET";
        case Opcode::IN: return "IN";
        case Opcode::OUT: return "OUT";
        case Opcode::HLT: return "HLT";
    }

    return "UNKNOWN";
}

std::optional<Opcode> opcodeFromMnemonic(const std::string& mnemonic) {
    static const std::unordered_map<std::string, Opcode> opcodes = {
        {"NOP", Opcode::NOP},
        {"MOV", Opcode::MOV},
        {"LDI", Opcode::LDI},
        {"LDR", Opcode::LDR},
        {"STR", Opcode::STR},
        {"ADD", Opcode::ADD},
        {"SUB", Opcode::SUB},
        {"INC", Opcode::INC},
        {"DEC", Opcode::DEC},
        {"AND", Opcode::AND},
        {"OR", Opcode::OR},
        {"XOR", Opcode::XOR},
        {"NOT", Opcode::NOT},
        {"SHL", Opcode::SHL},
        {"SLL", Opcode::SHL},
        {"SHR", Opcode::SHR},
        {"CMP", Opcode::CMP},
        {"JMP", Opcode::JMP},
        {"JZ", Opcode::JZ},
        {"JNZ", Opcode::JNZ},
        {"JC", Opcode::JC},
        {"JNC", Opcode::JNC},
        {"PUSH", Opcode::PUSH},
        {"POP", Opcode::POP},
        {"CALL", Opcode::CALL},
        {"JAL", Opcode::CALL},
        {"RET", Opcode::RET},
        {"IN", Opcode::IN},
        {"OUT", Opcode::OUT},
        {"HLT", Opcode::HLT},
        {"EXIT", Opcode::HLT}
    };

    const auto found = opcodes.find(mnemonic);
    if (found == opcodes.end()) {
        return std::nullopt;
    }

    return found->second;
}

std::uint32_t packInstruction(const Instruction& instruction) {
    return static_cast<std::uint32_t>(instruction.opcode)
        | (static_cast<std::uint32_t>(instruction.rd & 0x0F) << 8)
        | (static_cast<std::uint32_t>(instruction.rs & 0x0F) << 12)
        | (static_cast<std::uint32_t>(instruction.operand) << 16);
}

Instruction unpackInstruction(std::uint32_t word) {
    Instruction instruction;
    instruction.opcode = static_cast<Opcode>(word & 0xFF);
    instruction.rd = static_cast<std::uint8_t>((word >> 8) & 0x0F);
    instruction.rs = static_cast<std::uint8_t>((word >> 12) & 0x0F);
    instruction.operand = static_cast<std::uint16_t>((word >> 16) & 0xFFFF);
    return instruction;
}

std::vector<std::uint8_t> encodeInstruction(const Instruction& instruction) {
    const std::uint32_t word = packInstruction(instruction);
    return {
        static_cast<std::uint8_t>(word & 0xFF),
        static_cast<std::uint8_t>((word >> 8) & 0xFF),
        static_cast<std::uint8_t>((word >> 16) & 0xFF),
        static_cast<std::uint8_t>((word >> 24) & 0xFF)
    };
}

std::string formatInstruction(const Instruction& instruction) {
    std::ostringstream out;
    out << opcodeName(instruction.opcode);

    switch (instruction.opcode) {
        case Opcode::MOV:
        case Opcode::ADD:
        case Opcode::SUB:
        case Opcode::AND:
        case Opcode::OR:
        case Opcode::XOR:
        case Opcode::CMP:
            out << " R" << static_cast<int>(instruction.rd)
                << ", R" << static_cast<int>(instruction.rs);
            break;
        case Opcode::LDI:
            out << " R" << static_cast<int>(instruction.rd)
                << ", " << static_cast<int>(instruction.operand & 0xFF);
            break;
        case Opcode::INC:
        case Opcode::DEC:
        case Opcode::NOT:
        case Opcode::SHL:
        case Opcode::SHR:
        case Opcode::PUSH:
        case Opcode::POP:
            out << " R" << static_cast<int>(instruction.rd);
            break;
        case Opcode::LDR:
        case Opcode::STR:
        case Opcode::IN:
        case Opcode::OUT:
            out << " R" << static_cast<int>(instruction.rd)
                << ", [0x" << std::hex << std::uppercase << std::setw(4)
                << std::setfill('0') << instruction.operand << "]";
            break;
        case Opcode::JMP:
        case Opcode::JZ:
        case Opcode::JNZ:
        case Opcode::JC:
        case Opcode::JNC:
        case Opcode::CALL:
            out << " 0x" << std::hex << std::uppercase << std::setw(4)
                << std::setfill('0') << instruction.operand;
            break;
        default:
            break;
    }

    return out.str();
}

} // namespace softcpu

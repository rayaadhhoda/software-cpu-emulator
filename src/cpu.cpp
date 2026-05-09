#include "cpu.h"

#include <iomanip>
#include <ostream>
#include <sstream>
#include <stdexcept>

#include "alu.h"

namespace softcpu {

CPU::CPU(Bus& bus) : bus_(bus) {
    reset();
}

void CPU::reset() {
    registers_ = Registers{};
}

void CPU::setTrace(bool enabled) {
    trace_ = enabled;
}

bool CPU::step(std::ostream& traceOutput) {
    if (registers_.halted) {
        return false;
    }

    const std::uint16_t pcBefore = registers_.pc;
    registers_.ir = fetchWord();
    const Instruction instruction = unpackInstruction(registers_.ir);

    switch (instruction.opcode) {
        case Opcode::NOP:
            break;
        case Opcode::MOV:
            reg(instruction.rd) = reg(instruction.rs);
            setZeroNegative(reg(instruction.rd));
            break;
        case Opcode::LDI:
            reg(instruction.rd) = static_cast<std::uint8_t>(instruction.operand & 0xFF);
            setZeroNegative(reg(instruction.rd));
            break;
        case Opcode::LDR:
        case Opcode::IN:
            reg(instruction.rd) = bus_.read(instruction.operand);
            setZeroNegative(reg(instruction.rd));
            break;
        case Opcode::STR:
        case Opcode::OUT:
            bus_.write(instruction.operand, reg(instruction.rd));
            break;
        case Opcode::ADD: {
            const AluResult result = ALU::add(reg(instruction.rd), reg(instruction.rs));
            reg(instruction.rd) = result.value;
            registers_.flags = result.flags;
            break;
        }
        case Opcode::SUB: {
            const AluResult result = ALU::sub(reg(instruction.rd), reg(instruction.rs));
            reg(instruction.rd) = result.value;
            registers_.flags = result.flags;
            break;
        }
        case Opcode::INC: {
            const AluResult result = ALU::add(reg(instruction.rd), 1);
            reg(instruction.rd) = result.value;
            registers_.flags = result.flags;
            break;
        }
        case Opcode::DEC: {
            const AluResult result = ALU::sub(reg(instruction.rd), 1);
            reg(instruction.rd) = result.value;
            registers_.flags = result.flags;
            break;
        }
        case Opcode::AND: {
            const AluResult result = ALU::bitAnd(reg(instruction.rd), reg(instruction.rs));
            reg(instruction.rd) = result.value;
            registers_.flags = result.flags;
            break;
        }
        case Opcode::OR: {
            const AluResult result = ALU::bitOr(reg(instruction.rd), reg(instruction.rs));
            reg(instruction.rd) = result.value;
            registers_.flags = result.flags;
            break;
        }
        case Opcode::XOR: {
            const AluResult result = ALU::bitXor(reg(instruction.rd), reg(instruction.rs));
            reg(instruction.rd) = result.value;
            registers_.flags = result.flags;
            break;
        }
        case Opcode::NOT: {
            const AluResult result = ALU::bitNot(reg(instruction.rd));
            reg(instruction.rd) = result.value;
            registers_.flags = result.flags;
            break;
        }
        case Opcode::SHL: {
            const AluResult result = ALU::shiftLeft(reg(instruction.rd));
            reg(instruction.rd) = result.value;
            registers_.flags = result.flags;
            break;
        }
        case Opcode::SHR: {
            const AluResult result = ALU::shiftRight(reg(instruction.rd));
            reg(instruction.rd) = result.value;
            registers_.flags = result.flags;
            break;
        }
        case Opcode::CMP: {
            const AluResult result = ALU::sub(reg(instruction.rd), reg(instruction.rs));
            registers_.flags = result.flags;
            break;
        }
        case Opcode::JMP:
            branchIf(true, instruction.operand);
            break;
        case Opcode::JZ:
            branchIf((registers_.flags & FLAG_Z) != 0, instruction.operand);
            break;
        case Opcode::JNZ:
            branchIf((registers_.flags & FLAG_Z) == 0, instruction.operand);
            break;
        case Opcode::JC:
            branchIf((registers_.flags & FLAG_C) != 0, instruction.operand);
            break;
        case Opcode::JNC:
            branchIf((registers_.flags & FLAG_C) == 0, instruction.operand);
            break;
        case Opcode::PUSH:
            push8(reg(instruction.rd));
            break;
        case Opcode::POP:
            reg(instruction.rd) = pop8();
            setZeroNegative(reg(instruction.rd));
            break;
        case Opcode::CALL:
            push16(registers_.pc);
            registers_.pc = instruction.operand;
            break;
        case Opcode::RET:
            registers_.pc = pop16();
            break;
        case Opcode::HLT:
            registers_.halted = true;
            break;
        default:
            throw std::runtime_error("unknown opcode at PC 0x" + std::to_string(pcBefore));
    }

    if (trace_) {
        traceOutput << std::hex << std::uppercase << std::setfill('0')
            << "PC=0x" << std::setw(4) << pcBefore << "  "
            << std::setfill(' ')
            << std::left << std::setw(24) << formatInstruction(instruction) << std::right
            << "  " << formatRegisters() << '\n';
    }

    return !registers_.halted;
}

RunResult CPU::run(std::uint64_t maxCycles, std::ostream& traceOutput) {
    RunResult result;

    while (!registers_.halted && result.cycles < maxCycles) {
        step(traceOutput);
        ++result.cycles;
    }

    result.halted = registers_.halted;
    result.hitCycleLimit = !registers_.halted && result.cycles >= maxCycles;
    return result;
}

const Registers& CPU::registers() const {
    return registers_;
}

std::string CPU::formatRegisters() const {
    std::ostringstream out;
    out << std::dec
        << "R0=" << static_cast<int>(registers_.r[0]) << ' '
        << "R1=" << static_cast<int>(registers_.r[1]) << ' '
        << "R2=" << static_cast<int>(registers_.r[2]) << ' '
        << "R3=" << static_cast<int>(registers_.r[3]) << ' '
        << std::hex << std::uppercase << std::setfill('0')
        << "PC=0x" << std::setw(4) << registers_.pc << ' '
        << "SP=0x" << std::setw(4) << registers_.sp << ' '
        << "FLAGS="
        << ((registers_.flags & FLAG_Z) ? 'Z' : '-')
        << ((registers_.flags & FLAG_C) ? 'C' : '-')
        << ((registers_.flags & FLAG_N) ? 'N' : '-')
        << ((registers_.flags & FLAG_V) ? 'V' : '-');
    return out.str();
}

std::uint32_t CPU::fetchWord() {
    const std::uint16_t pc = registers_.pc;
    const std::uint32_t b0 = bus_.read(pc);
    const std::uint32_t b1 = bus_.read(static_cast<std::uint16_t>(pc + 1));
    const std::uint32_t b2 = bus_.read(static_cast<std::uint16_t>(pc + 2));
    const std::uint32_t b3 = bus_.read(static_cast<std::uint16_t>(pc + 3));
    registers_.pc = static_cast<std::uint16_t>(registers_.pc + INSTRUCTION_SIZE);
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

std::uint8_t& CPU::reg(std::uint8_t index) {
    if (index >= REGISTER_COUNT) {
        throw std::runtime_error("invalid register index");
    }
    return registers_.r[index];
}

std::uint8_t CPU::reg(std::uint8_t index) const {
    if (index >= REGISTER_COUNT) {
        throw std::runtime_error("invalid register index");
    }
    return registers_.r[index];
}

void CPU::setZeroNegative(std::uint8_t value) {
    registers_.flags = ALU::zeroNegativeFlags(value);
}

void CPU::push8(std::uint8_t value) {
    bus_.write(registers_.sp, value);
    --registers_.sp;
}

std::uint8_t CPU::pop8() {
    ++registers_.sp;
    return bus_.read(registers_.sp);
}

void CPU::push16(std::uint16_t value) {
    push8(static_cast<std::uint8_t>((value >> 8) & 0xFF));
    push8(static_cast<std::uint8_t>(value & 0xFF));
}

std::uint16_t CPU::pop16() {
    const std::uint8_t low = pop8();
    const std::uint8_t high = pop8();
    return static_cast<std::uint16_t>(low | (high << 8));
}

bool CPU::branchIf(bool condition, std::uint16_t address) {
    if (condition) {
        registers_.pc = address;
    }
    return condition;
}

} // namespace softcpu

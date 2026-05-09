#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace softcpu {

constexpr std::uint32_t MEMORY_SIZE = 65536;
constexpr std::uint16_t PROGRAM_START = 0x0000;
constexpr std::uint16_t STACK_START = 0xDFFF;

constexpr std::uint16_t IO_CONSOLE_CHAR = 0xE000;
constexpr std::uint16_t IO_CONSOLE_NUMBER = 0xE001;
constexpr std::uint16_t IO_TIMER_LOW = 0xE002;
constexpr std::uint16_t IO_TIMER_HIGH = 0xE003;
constexpr std::uint16_t IO_STATUS = 0xE004;

constexpr std::uint8_t REGISTER_COUNT = 4;
constexpr std::uint8_t INSTRUCTION_SIZE = 4;

enum class Opcode : std::uint8_t {
    NOP = 0x00,
    MOV = 0x01,
    LDI = 0x02,
    LDR = 0x03,
    STR = 0x04,
    ADD = 0x05,
    SUB = 0x06,
    INC = 0x07,
    DEC = 0x08,
    AND = 0x09,
    OR = 0x0A,
    XOR = 0x0B,
    NOT = 0x0C,
    SHL = 0x0D,
    SHR = 0x0E,
    CMP = 0x0F,
    JMP = 0x10,
    JZ = 0x11,
    JNZ = 0x12,
    JC = 0x13,
    JNC = 0x14,
    PUSH = 0x15,
    POP = 0x16,
    CALL = 0x17,
    RET = 0x18,
    IN = 0x19,
    OUT = 0x1A,
    HLT = 0xFF
};

enum Flag : std::uint8_t {
    FLAG_Z = 1u << 0,
    FLAG_C = 1u << 1,
    FLAG_N = 1u << 2,
    FLAG_V = 1u << 3
};

struct Instruction {
    Opcode opcode = Opcode::NOP;
    std::uint8_t rd = 0;
    std::uint8_t rs = 0;
    std::uint16_t operand = 0;
};

std::string opcodeName(Opcode opcode);
std::optional<Opcode> opcodeFromMnemonic(const std::string& mnemonic);
std::uint32_t packInstruction(const Instruction& instruction);
Instruction unpackInstruction(std::uint32_t word);
std::vector<std::uint8_t> encodeInstruction(const Instruction& instruction);
std::string formatInstruction(const Instruction& instruction);

} // namespace softcpu

#include "assembler.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace softcpu {

AssemblerResult Assembler::assembleFile(const std::string& inputPath) const {
    std::ifstream input(inputPath);
    if (!input) {
        throw std::runtime_error("could not open assembly file: " + inputPath);
    }

    std::ostringstream buffer;
    buffer << input.rdbuf();
    return assembleText(buffer.str(), inputPath);
}

AssemblerResult Assembler::assembleText(const std::string& source, const std::string& sourceName) const {
    std::unordered_map<std::string, std::uint16_t> labels;
    std::vector<SourceLine> instructions;
    std::istringstream input(source);
    std::string rawLine;
    std::uint16_t address = PROGRAM_START;
    int lineNumber = 0;

    while (std::getline(input, rawLine)) {
        ++lineNumber;
        std::string line = trim(stripComment(rawLine));
        if (line.empty() || isDirective(line)) {
            continue;
        }

        while (true) {
            const std::size_t colon = line.find(':');
            if (colon == std::string::npos) {
                break;
            }

            const std::string label = upper(trim(line.substr(0, colon)));
            const bool validLabel = !label.empty() && std::all_of(label.begin(), label.end(), [](unsigned char ch) {
                return std::isalnum(ch) != 0 || ch == '_';
            });
            if (!validLabel) {
                break;
            }

            if (label.empty()) {
                throw std::runtime_error(sourceName + ":" + std::to_string(lineNumber) + ": empty label");
            }
            if (labels.find(label) != labels.end()) {
                throw std::runtime_error(sourceName + ":" + std::to_string(lineNumber) + ": duplicate label " + label);
            }

            labels[label] = address;
            line = trim(line.substr(colon + 1));
            if (line.empty()) {
                break;
            }
        }

        if (!line.empty()) {
            instructions.push_back({line, address, lineNumber});
            address = static_cast<std::uint16_t>(address + INSTRUCTION_SIZE);
        }
    }

    AssemblerResult result;
    for (const SourceLine& sourceLine : instructions) {
        const Instruction instruction = parseInstruction(sourceLine, labels);
        const std::vector<std::uint8_t> bytes = encodeInstruction(instruction);
        result.machineCode.insert(result.machineCode.end(), bytes.begin(), bytes.end());

        std::ostringstream listingLine;
        listingLine << std::hex << std::uppercase << std::setw(4) << std::setfill('0')
                    << sourceLine.address << "  " << formatInstruction(instruction);
        result.listing.push_back(listingLine.str());
    }

    return result;
}

void Assembler::writeBinary(const std::string& outputPath, const std::vector<std::uint8_t>& machineCode) const {
    std::ofstream output(outputPath, std::ios::binary);
    if (!output) {
        throw std::runtime_error("could not open output file: " + outputPath);
    }

    output.write(reinterpret_cast<const char*>(machineCode.data()), static_cast<std::streamsize>(machineCode.size()));
}

std::string Assembler::stripComment(const std::string& line) {
    bool inCharLiteral = false;
    bool escaped = false;

    for (std::size_t i = 0; i < line.size(); ++i) {
        const char ch = line[i];
        if (escaped) {
            escaped = false;
            continue;
        }
        if (ch == '\\') {
            escaped = true;
            continue;
        }
        if (ch == '\'') {
            inCharLiteral = !inCharLiteral;
            continue;
        }
        if (ch == ';' && !inCharLiteral) {
            return line.substr(0, i);
        }
    }

    return line;
}

std::string Assembler::trim(const std::string& value) {
    const auto first = std::find_if_not(value.begin(), value.end(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    });
    if (first == value.end()) {
        return "";
    }

    const auto last = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    }).base();
    return std::string(first, last);
}

std::string Assembler::upper(const std::string& value) {
    std::string result = value;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char ch) {
        return static_cast<char>(std::toupper(ch));
    });
    return result;
}

std::vector<std::string> Assembler::splitOperands(const std::string& text) {
    std::vector<std::string> operands;
    std::string current;
    bool inCharLiteral = false;
    bool escaped = false;

    for (char ch : text) {
        if (escaped) {
            current.push_back(ch);
            escaped = false;
            continue;
        }
        if (ch == '\\') {
            current.push_back(ch);
            escaped = true;
            continue;
        }
        if (ch == '\'') {
            current.push_back(ch);
            inCharLiteral = !inCharLiteral;
            continue;
        }
        if (ch == ',' && !inCharLiteral) {
            operands.push_back(trim(current));
            current.clear();
            continue;
        }
        current.push_back(ch);
    }

    if (!trim(current).empty()) {
        operands.push_back(trim(current));
    }

    return operands;
}

bool Assembler::isDirective(const std::string& text) {
    return !text.empty() && text[0] == '.';
}

bool Assembler::parseRegister(const std::string& text, std::uint8_t& out) {
    std::string value = upper(trim(text));
    if (!value.empty() && value[0] == '%') {
        value.erase(value.begin());
    }

    if (value.size() == 2 && value[0] == 'R' && value[1] >= '0' && value[1] <= '3') {
        out = static_cast<std::uint8_t>(value[1] - '0');
        return true;
    }

    return false;
}

std::string Assembler::unwrapMemoryOperand(const std::string& text) {
    std::string value = trim(text);
    if (value.size() >= 2 && value.front() == '[' && value.back() == ']') {
        return trim(value.substr(1, value.size() - 2));
    }
    return value;
}

std::uint16_t Assembler::parseValue(
    const std::string& text,
    const std::unordered_map<std::string, std::uint16_t>& labels,
    int lineNumber) {
    std::string value = trim(text);
    if (!value.empty() && value[0] == '#') {
        value.erase(value.begin());
        value = trim(value);
    }

    if (value.empty()) {
        throw std::runtime_error("line " + std::to_string(lineNumber) + ": missing numeric value");
    }

    if (value.size() >= 3 && value.front() == '\'' && value.back() == '\'') {
        if (value.size() == 3) {
            return static_cast<std::uint8_t>(value[1]);
        }
        if (value.size() == 4 && value[1] == '\\') {
            switch (value[2]) {
                case 'n': return '\n';
                case 'r': return '\r';
                case 't': return '\t';
                case '0': return '\0';
                case '\\': return '\\';
                case '\'': return '\'';
                default:
                    throw std::runtime_error("line " + std::to_string(lineNumber) + ": unknown escape sequence");
            }
        }
        throw std::runtime_error("line " + std::to_string(lineNumber) + ": invalid character literal");
    }

    const std::string key = upper(value);
    const auto label = labels.find(key);
    if (label != labels.end()) {
        return label->second;
    }

    int base = 10;
    std::string digits = value;
    if (digits.size() > 2 && digits[0] == '0' && (digits[1] == 'x' || digits[1] == 'X')) {
        base = 16;
        digits = digits.substr(2);
    } else if (digits.size() > 2 && digits[0] == '0' && (digits[1] == 'b' || digits[1] == 'B')) {
        base = 2;
        digits = digits.substr(2);
    }

    try {
        std::size_t processed = 0;
        const unsigned long parsed = std::stoul(digits, &processed, base);
        if (processed != digits.size() || parsed > 0xFFFF) {
            throw std::runtime_error("");
        }
        return static_cast<std::uint16_t>(parsed);
    } catch (...) {
        throw std::runtime_error("line " + std::to_string(lineNumber) + ": unknown label or invalid value " + value);
    }
}

Instruction Assembler::parseInstruction(
    const SourceLine& line,
    const std::unordered_map<std::string, std::uint16_t>& labels) {
    std::istringstream input(line.text);
    std::string mnemonic;
    input >> mnemonic;
    mnemonic = upper(mnemonic);

    const auto opcode = opcodeFromMnemonic(mnemonic);
    if (!opcode) {
        throw std::runtime_error("line " + std::to_string(line.lineNumber) + ": unknown instruction " + mnemonic);
    }

    std::string operandText;
    std::getline(input, operandText);
    const std::vector<std::string> operands = splitOperands(operandText);
    Instruction instruction;
    instruction.opcode = *opcode;

    auto requireCount = [&](std::size_t expected) {
        if (operands.size() != expected) {
            throw std::runtime_error(
                "line " + std::to_string(line.lineNumber) + ": " + mnemonic
                + " expects " + std::to_string(expected) + " operand(s)");
        }
    };
    auto requireRegister = [&](const std::string& value) {
        std::uint8_t reg = 0;
        if (!parseRegister(value, reg)) {
            throw std::runtime_error("line " + std::to_string(line.lineNumber) + ": expected register, got " + value);
        }
        return reg;
    };

    switch (*opcode) {
        case Opcode::NOP:
        case Opcode::RET:
        case Opcode::HLT:
            requireCount(0);
            break;
        case Opcode::MOV:
        case Opcode::ADD:
        case Opcode::SUB:
        case Opcode::AND:
        case Opcode::OR:
        case Opcode::XOR:
        case Opcode::CMP:
            requireCount(2);
            instruction.rd = requireRegister(operands[0]);
            instruction.rs = requireRegister(operands[1]);
            break;
        case Opcode::LDI:
            requireCount(2);
            instruction.rd = requireRegister(operands[0]);
            instruction.operand = parseValue(operands[1], labels, line.lineNumber);
            if (instruction.operand > 0xFF) {
                throw std::runtime_error("line " + std::to_string(line.lineNumber) + ": LDI immediate must fit in 8 bits");
            }
            break;
        case Opcode::INC:
        case Opcode::DEC:
        case Opcode::NOT:
        case Opcode::SHL:
        case Opcode::SHR:
        case Opcode::PUSH:
        case Opcode::POP:
            requireCount(1);
            instruction.rd = requireRegister(operands[0]);
            break;
        case Opcode::LDR:
        case Opcode::STR:
        case Opcode::IN:
        case Opcode::OUT:
            requireCount(2);
            instruction.rd = requireRegister(operands[0]);
            instruction.operand = parseValue(unwrapMemoryOperand(operands[1]), labels, line.lineNumber);
            break;
        case Opcode::JMP:
        case Opcode::JZ:
        case Opcode::JNZ:
        case Opcode::JC:
        case Opcode::JNC:
        case Opcode::CALL:
            requireCount(1);
            instruction.operand = parseValue(operands[0], labels, line.lineNumber);
            break;
        default:
            throw std::runtime_error("line " + std::to_string(line.lineNumber) + ": unsupported instruction");
    }

    return instruction;
}

} // namespace softcpu

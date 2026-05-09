#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "isa.h"

namespace softcpu {

struct AssemblerResult {
    std::vector<std::uint8_t> machineCode;
    std::vector<std::string> listing;
};

class Assembler {
public:
    AssemblerResult assembleFile(const std::string& inputPath) const;
    AssemblerResult assembleText(const std::string& source, const std::string& sourceName = "<memory>") const;
    void writeBinary(const std::string& outputPath, const std::vector<std::uint8_t>& machineCode) const;

private:
    struct SourceLine {
        std::string text;
        std::uint16_t address = 0;
        int lineNumber = 0;
    };

    static std::string stripComment(const std::string& line);
    static std::string trim(const std::string& value);
    static std::string upper(const std::string& value);
    static std::vector<std::string> splitOperands(const std::string& text);
    static bool isDirective(const std::string& text);
    static bool parseRegister(const std::string& text, std::uint8_t& out);
    static std::string unwrapMemoryOperand(const std::string& text);
    static std::uint16_t parseValue(
        const std::string& text,
        const std::unordered_map<std::string, std::uint16_t>& labels,
        int lineNumber);
    static Instruction parseInstruction(
        const SourceLine& line,
        const std::unordered_map<std::string, std::uint16_t>& labels);
};

} // namespace softcpu

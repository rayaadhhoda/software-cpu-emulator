#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "assembler.h"
#include "bus.h"
#include "cpu.h"
#include "memory.h"

using namespace softcpu;

namespace {

void printUsage() {
    std::cout
        << "Software CPU\n"
        << "\n"
        << "Usage:\n"
        << "  software_cpu assemble <input.asm> <output.bin>\n"
        << "  software_cpu run <program.bin> [--trace] [--max-cycles N]\n"
        << "  software_cpu step <program.bin> [--steps N]\n"
        << "  software_cpu dump <program.bin> <start> <end>\n"
        << "\n"
        << "Examples:\n"
        << "  software_cpu assemble programs/hello.asm roms/hello.bin\n"
        << "  software_cpu run roms/hello.bin\n"
        << "  software_cpu run roms/fibonacci.bin --trace\n";
}

std::vector<std::uint8_t> readBinary(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        throw std::runtime_error("could not open binary file: " + path);
    }

    return std::vector<std::uint8_t>(
        std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>());
}

std::uint16_t parseAddress(const std::string& text) {
    int base = 10;
    std::string value = text;
    if (value.size() > 2 && value[0] == '0' && (value[1] == 'x' || value[1] == 'X')) {
        base = 16;
        value = value.substr(2);
    }

    std::size_t processed = 0;
    const unsigned long parsed = std::stoul(value, &processed, base);
    if (processed != value.size() || parsed > 0xFFFF) {
        throw std::runtime_error("invalid address: " + text);
    }

    return static_cast<std::uint16_t>(parsed);
}

std::uint64_t parseUnsigned(const std::string& text) {
    std::size_t processed = 0;
    const unsigned long long parsed = std::stoull(text, &processed, 10);
    if (processed != text.size()) {
        throw std::runtime_error("invalid number: " + text);
    }
    return parsed;
}

int assembleCommand(int argc, char* argv[]) {
    if (argc != 4) {
        printUsage();
        return 1;
    }

    Assembler assembler;
    const AssemblerResult result = assembler.assembleFile(argv[2]);
    assembler.writeBinary(argv[3], result.machineCode);

    for (const std::string& line : result.listing) {
        std::cout << line << '\n';
    }
    std::cout << "Wrote " << result.machineCode.size() << " byte(s) to " << argv[3] << '\n';
    return 0;
}

int runCommand(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage();
        return 1;
    }

    bool trace = false;
    bool explicitCycleLimit = false;
    std::uint64_t maxCycles = 100000;

    for (int i = 3; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--trace") {
            trace = true;
        } else if (arg == "--max-cycles" && i + 1 < argc) {
            explicitCycleLimit = true;
            maxCycles = parseUnsigned(argv[++i]);
        } else {
            throw std::runtime_error("unknown run option: " + arg);
        }
    }

    Memory memory;
    Bus bus(memory, std::cout);
    CPU cpu(bus);
    cpu.setTrace(trace);
    bus.loadProgram(readBinary(argv[2]));

    const RunResult result = cpu.run(maxCycles, std::cout);
    if (result.hitCycleLimit) {
        std::cout << "\nStopped after cycle limit: " << result.cycles << '\n';
    } else {
        std::cout << "\nHalted after " << result.cycles << " cycle(s)\n";
    }
    std::cout << cpu.formatRegisters() << '\n';
    return result.hitCycleLimit && !explicitCycleLimit ? 2 : 0;
}

int stepCommand(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage();
        return 1;
    }

    std::uint64_t steps = 1;
    for (int i = 3; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--steps" && i + 1 < argc) {
            steps = parseUnsigned(argv[++i]);
        } else {
            throw std::runtime_error("unknown step option: " + arg);
        }
    }

    Memory memory;
    Bus bus(memory, std::cout);
    CPU cpu(bus);
    cpu.setTrace(true);
    bus.loadProgram(readBinary(argv[2]));

    for (std::uint64_t i = 0; i < steps && !cpu.registers().halted; ++i) {
        cpu.step(std::cout);
    }
    return 0;
}

int dumpCommand(int argc, char* argv[]) {
    if (argc != 5) {
        printUsage();
        return 1;
    }

    Memory memory;
    Bus bus(memory, std::cout);
    bus.loadProgram(readBinary(argv[2]));

    const std::uint16_t start = parseAddress(argv[3]);
    const std::uint16_t end = parseAddress(argv[4]);
    std::cout << bus.dump(start, end);
    return 0;
}

} // namespace

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            printUsage();
            return 1;
        }

        const std::string command = argv[1];
        if (command == "assemble") {
            return assembleCommand(argc, argv);
        }
        if (command == "run") {
            return runCommand(argc, argv);
        }
        if (command == "step") {
            return stepCommand(argc, argv);
        }
        if (command == "dump") {
            return dumpCommand(argc, argv);
        }
        if (command == "help" || command == "--help" || command == "-h") {
            printUsage();
            return 0;
        }

        std::cerr << "Unknown command: " << command << "\n\n";
        printUsage();
        return 1;
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << '\n';
        return 1;
    }
}

#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>

#include "bus.h"
#include "isa.h"

namespace softcpu {

struct Registers {
    std::uint8_t r[REGISTER_COUNT] = {0, 0, 0, 0};
    std::uint16_t pc = PROGRAM_START;
    std::uint16_t sp = STACK_START;
    std::uint32_t ir = 0;
    std::uint8_t flags = 0;
    bool halted = false;
};

struct RunResult {
    std::uint64_t cycles = 0;
    bool halted = false;
    bool hitCycleLimit = false;
};

class CPU {
public:
    explicit CPU(Bus& bus);

    void reset();
    void setTrace(bool enabled);
    bool step(std::ostream& traceOutput);
    RunResult run(std::uint64_t maxCycles, std::ostream& traceOutput);
    const Registers& registers() const;
    std::string formatRegisters() const;

private:
    std::uint32_t fetchWord();
    std::uint8_t& reg(std::uint8_t index);
    std::uint8_t reg(std::uint8_t index) const;
    void setZeroNegative(std::uint8_t value);
    void push8(std::uint8_t value);
    std::uint8_t pop8();
    void push16(std::uint16_t value);
    std::uint16_t pop16();
    bool branchIf(bool condition, std::uint16_t address);

    Bus& bus_;
    Registers registers_;
    bool trace_ = false;
};

} // namespace softcpu

#pragma once

#include <cstdint>
#include <iosfwd>
#include <vector>

#include "isa.h"
#include "memory.h"

namespace softcpu {

class Bus {
public:
    Bus(Memory& memory, std::ostream& output);

    std::uint8_t read(std::uint16_t address);
    void write(std::uint16_t address, std::uint8_t value);
    void loadProgram(const std::vector<std::uint8_t>& bytes, std::uint16_t startAddress = PROGRAM_START);
    std::string dump(std::uint16_t startAddress, std::uint16_t endAddress) const;

private:
    bool isIoAddress(std::uint16_t address) const;

    Memory& memory_;
    std::ostream& output_;
    std::uint8_t timerLow_ = 0;
    std::uint8_t timerHigh_ = 0;
    std::uint8_t status_ = 0;
};

} // namespace softcpu

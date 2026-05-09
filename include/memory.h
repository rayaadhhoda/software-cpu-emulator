#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "isa.h"

namespace softcpu {

class Memory {
public:
    Memory();

    void clear();
    std::uint8_t read(std::uint16_t address) const;
    void write(std::uint16_t address, std::uint8_t value);
    void load(const std::vector<std::uint8_t>& bytes, std::uint16_t startAddress = PROGRAM_START);
    std::string dump(std::uint16_t startAddress, std::uint16_t endAddress) const;

private:
    std::vector<std::uint8_t> bytes_;
};

} // namespace softcpu

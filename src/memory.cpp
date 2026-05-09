#include "memory.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace softcpu {

Memory::Memory() : bytes_(MEMORY_SIZE, 0) {}

void Memory::clear() {
    std::fill(bytes_.begin(), bytes_.end(), 0);
}

std::uint8_t Memory::read(std::uint16_t address) const {
    return bytes_[address];
}

void Memory::write(std::uint16_t address, std::uint8_t value) {
    bytes_[address] = value;
}

void Memory::load(const std::vector<std::uint8_t>& bytes, std::uint16_t startAddress) {
    if (static_cast<std::uint32_t>(startAddress) + bytes.size() > bytes_.size()) {
        throw std::runtime_error("program does not fit in memory");
    }

    std::copy(bytes.begin(), bytes.end(), bytes_.begin() + startAddress);
}

std::string Memory::dump(std::uint16_t startAddress, std::uint16_t endAddress) const {
    std::ostringstream out;
    const std::uint32_t start = startAddress;
    const std::uint32_t end = endAddress;

    if (end < start) {
        throw std::runtime_error("dump end address is before start address");
    }

    for (std::uint32_t row = start; row <= end; row += 16) {
        out << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << row << ": ";

        for (std::uint32_t offset = 0; offset < 16 && row + offset <= end; ++offset) {
            out << std::setw(2) << static_cast<int>(bytes_[row + offset]) << ' ';
        }

        out << '\n';
    }

    return out.str();
}

} // namespace softcpu

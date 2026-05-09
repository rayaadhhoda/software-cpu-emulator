#include "bus.h"

#include <ostream>

namespace softcpu {

Bus::Bus(Memory& memory, std::ostream& output) : memory_(memory), output_(output) {}

std::uint8_t Bus::read(std::uint16_t address) {
    switch (address) {
        case IO_CONSOLE_CHAR:
        case IO_CONSOLE_NUMBER:
            return 0;
        case IO_TIMER_LOW:
            return timerLow_;
        case IO_TIMER_HIGH:
            return timerHigh_;
        case IO_STATUS:
            return status_;
        default:
            return memory_.read(address);
    }
}

void Bus::write(std::uint16_t address, std::uint8_t value) {
    switch (address) {
        case IO_CONSOLE_CHAR:
            output_ << static_cast<char>(value);
            return;
        case IO_CONSOLE_NUMBER:
            output_ << static_cast<int>(value) << ' ';
            return;
        case IO_TIMER_LOW:
            timerLow_ = value;
            output_ << "[timer] " << static_cast<int>(timerLow_) << '\n';
            return;
        case IO_TIMER_HIGH:
            timerHigh_ = value;
            return;
        case IO_STATUS:
            status_ = value;
            return;
        default:
            memory_.write(address, value);
            return;
    }
}

void Bus::loadProgram(const std::vector<std::uint8_t>& bytes, std::uint16_t startAddress) {
    memory_.load(bytes, startAddress);
}

std::string Bus::dump(std::uint16_t startAddress, std::uint16_t endAddress) const {
    return memory_.dump(startAddress, endAddress);
}

bool Bus::isIoAddress(std::uint16_t address) const {
    return address >= IO_CONSOLE_CHAR && address <= IO_STATUS;
}

} // namespace softcpu

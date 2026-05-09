#include "alu.h"

namespace softcpu {

std::uint8_t ALU::zeroNegativeFlags(std::uint8_t value) {
    std::uint8_t flags = 0;
    if (value == 0) {
        flags |= FLAG_Z;
    }
    if ((value & 0x80) != 0) {
        flags |= FLAG_N;
    }
    return flags;
}

AluResult ALU::add(std::uint8_t lhs, std::uint8_t rhs) {
    const std::uint16_t wide = static_cast<std::uint16_t>(lhs) + rhs;
    const std::uint8_t value = static_cast<std::uint8_t>(wide & 0xFF);
    std::uint8_t flags = zeroNegativeFlags(value);

    if (wide > 0xFF) {
        flags |= FLAG_C;
    }
    if (((~(lhs ^ rhs) & (lhs ^ value)) & 0x80) != 0) {
        flags |= FLAG_V;
    }

    return {value, flags};
}

AluResult ALU::sub(std::uint8_t lhs, std::uint8_t rhs) {
    const std::uint16_t wide = static_cast<std::uint16_t>(lhs) - rhs;
    const std::uint8_t value = static_cast<std::uint8_t>(wide & 0xFF);
    std::uint8_t flags = zeroNegativeFlags(value);

    if (lhs < rhs) {
        flags |= FLAG_C;
    }
    if ((((lhs ^ rhs) & (lhs ^ value)) & 0x80) != 0) {
        flags |= FLAG_V;
    }

    return {value, flags};
}

AluResult ALU::bitAnd(std::uint8_t lhs, std::uint8_t rhs) {
    const std::uint8_t value = static_cast<std::uint8_t>(lhs & rhs);
    return {value, zeroNegativeFlags(value)};
}

AluResult ALU::bitOr(std::uint8_t lhs, std::uint8_t rhs) {
    const std::uint8_t value = static_cast<std::uint8_t>(lhs | rhs);
    return {value, zeroNegativeFlags(value)};
}

AluResult ALU::bitXor(std::uint8_t lhs, std::uint8_t rhs) {
    const std::uint8_t value = static_cast<std::uint8_t>(lhs ^ rhs);
    return {value, zeroNegativeFlags(value)};
}

AluResult ALU::bitNot(std::uint8_t value) {
    const std::uint8_t result = static_cast<std::uint8_t>(~value);
    return {result, zeroNegativeFlags(result)};
}

AluResult ALU::shiftLeft(std::uint8_t value) {
    const std::uint8_t result = static_cast<std::uint8_t>((value << 1) & 0xFF);
    std::uint8_t flags = zeroNegativeFlags(result);
    if ((value & 0x80) != 0) {
        flags |= FLAG_C;
    }
    return {result, flags};
}

AluResult ALU::shiftRight(std::uint8_t value) {
    const std::uint8_t result = static_cast<std::uint8_t>(value >> 1);
    std::uint8_t flags = zeroNegativeFlags(result);
    if ((value & 0x01) != 0) {
        flags |= FLAG_C;
    }
    return {result, flags};
}

} // namespace softcpu

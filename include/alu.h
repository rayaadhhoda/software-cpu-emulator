#pragma once

#include <cstdint>

#include "isa.h"

namespace softcpu {

struct AluResult {
    std::uint8_t value = 0;
    std::uint8_t flags = 0;
};

class ALU {
public:
    static AluResult add(std::uint8_t lhs, std::uint8_t rhs);
    static AluResult sub(std::uint8_t lhs, std::uint8_t rhs);
    static AluResult bitAnd(std::uint8_t lhs, std::uint8_t rhs);
    static AluResult bitOr(std::uint8_t lhs, std::uint8_t rhs);
    static AluResult bitXor(std::uint8_t lhs, std::uint8_t rhs);
    static AluResult bitNot(std::uint8_t value);
    static AluResult shiftLeft(std::uint8_t value);
    static AluResult shiftRight(std::uint8_t value);
    static std::uint8_t zeroNegativeFlags(std::uint8_t value);
};

} // namespace softcpu

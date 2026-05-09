// Convenience unity build for simple one-file compilation commands.
// Preferred project build: compile the files in src/ with include/ on the include path.
//
// Example:
//   g++ -std=c++17 -Iinclude emulator.cpp -o software_cpu

#include "src/alu.cpp"
#include "src/assembler.cpp"
#include "src/bus.cpp"
#include "src/cpu.cpp"
#include "src/isa.cpp"
#include "src/main.cpp"
#include "src/memory.cpp"

# Software CPU in C/C++

## Overview

This project implements a complete software CPU in C/C++. The goal is to model the core parts of a real computer system in a clean and understandable codebase. The project includes:

- A CPU schematic that shows the architecture at a high level
- A custom instruction set architecture
- A software emulator for the CPU
- An assembler for converting assembly source into machine code
- Memory mapped IO support
- Tools to load programs, run them, and inspect memory
- Example programs including Hello, World and Fibonacci Sequence
- A timer example that explains execution through Fetch, Compute, and Store cycles

The code is organized so that each subsystem is separated into clear modules. The design favors readability, testability, and simple extension.

## Project Goals

- Build a small but complete CPU model in software
- Define a compact and usable ISA
- Simulate registers, memory, ALU, control flow, and IO
- Provide an assembler for writing and testing programs
- Demonstrate the CPU with real example programs
- Keep the implementation easy to follow for learning and maintenance

## Suggested Repository Structure

```text
software-cpu/
├── README.md
├── LICENSE
├── docs/
│   ├── cpu_schematic.png
│   ├── isa.md
│   ├── memory_map.md
│   └── execution_cycle.md
├── include/
│   ├── cpu.h
│   ├── alu.h
│   ├── memory.h
│   ├── bus.h
│   ├── control_unit.h
│   ├── isa.h
│   ├── assembler.h
│   └── io.h
├── src/
│   ├── main.cpp
│   ├── cpu.cpp
│   ├── alu.cpp
│   ├── memory.cpp
│   ├── bus.cpp
│   ├── control_unit.cpp
│   ├── isa.cpp
│   ├── assembler.cpp
│   └── io.cpp
├── programs/
│   ├── hello.asm
│   ├── fibonacci.asm
│   └── timer_demo.asm
├── roms/
│   ├── hello.bin
│   ├── fibonacci.bin
│   └── timer_demo.bin
├── tests/
│   ├── test_alu.cpp
│   ├── test_cpu.cpp
│   ├── test_assembler.cpp
│   └── test_programs.cpp
└── video/
    └── fibonacci_demo.mp4
```

## CPU Architecture

The CPU is designed as a simple 8 bit processor with 16 bit addressing. This makes the implementation small enough to study while still supporting interesting programs.

### Main Components

- General purpose register file
- Program counter
- Stack pointer
- Flags register
- Arithmetic logic unit
- Control unit
- Address and data bus
- RAM and ROM abstraction
- Memory mapped IO region

### CPU Schematic

A simple architecture diagram should be included in `docs/cpu_schematic.png`.

Suggested block layout:

```text
                +----------------------+
                |     Control Unit     |
                +----------+-----------+
                           |
                           v
+-------------+   +------------------+   +-------------+
| Registers    |<->|       ALU        |<->| Flags Reg   |
| R0 R1 R2 R3  |   +------------------+   | Z C N V     |
| SP PC IR     |                          +-------------+
+------+------+ 
       |
       v
+------------------------------+
|          Internal Bus        |
+---------------+--------------+
                |
        +-------+--------+
        |                |
        v                v
+---------------+   +----------------+
|    Memory     |   | Memory Mapped  |
| ROM and RAM   |   | IO Devices     |
+---------------+   +----------------+
```

## ISA Design

The ISA is intentionally small and regular. Instructions use fixed width encoding for easier decoding.

### Word Size and Addressing

- Data width: 8 bits
- Address width: 16 bits
- Memory size: 64 KB
- Instruction width: 16 bits

### Registers

#### General Purpose Registers

- `R0`
- `R1`
- `R2`
- `R3`

#### Special Registers

- `PC` program counter
- `SP` stack pointer
- `IR` instruction register
- `FLAGS` condition flags register

### Flag Semantics

The CPU updates flags after arithmetic, logic, and compare operations.

- `Z` Zero flag. Set when result is zero
- `C` Carry flag. Set when addition carries out or subtraction borrows depending on design choice
- `N` Negative flag. Mirrors the top bit of the result
- `V` Overflow flag. Set on signed overflow

Suggested behavior:

- `MOV` may update `Z` and `N`
- `ADD`, `SUB` update `Z`, `C`, `N`, `V`
- `AND`, `OR`, `XOR` update `Z`, `N`, clear or preserve `C` and `V` by convention
- `CMP` updates flags but does not store the result
- Load and store instructions normally do not affect flags unless you explicitly choose to do so

## Instruction Format

Use a 16 bit fixed width format to keep the decoder simple.

### Format A: Register to Register

```text
15      12 11      8 7       6 5       4 3       0
+---------+---------+---------+---------+---------+
| opcode   |   rd    |   rs    |  mode   | unused  |
+---------+---------+---------+---------+---------+
```

### Format B: Register with Immediate

```text
15      12 11      8 7                          0
+---------+---------+----------------------------+
| opcode   |   rd    |        imm8                |
+---------+---------+----------------------------+
```

### Format C: Jump or Memory Address

```text
15      12 11                                   0
+---------+--------------------------------------+
| opcode   |            addr12                    |
+---------+--------------------------------------+
```

You can also choose to decode a second word for full 16 bit addresses if needed.

## Instruction Set

A minimal but useful instruction set might look like this:

### Data Movement

- `NOP` no operation
- `MOV rd, rs` copy register to register
- `LDI rd, imm8` load immediate into register
- `LDR rd, [addr]` load byte from memory
- `STR rs, [addr]` store byte to memory
- `PUSH rs` push register to stack
- `POP rd` pop register from stack

### Arithmetic

- `ADD rd, rs`
- `SUB rd, rs`
- `INC rd`
- `DEC rd`
- `MUL rd, rs` optional
- `DIV rd, rs` optional

### Logic

- `AND rd, rs`
- `OR rd, rs`
- `XOR rd, rs`
- `NOT rd`
- `SHL rd`
- `SHR rd`

### Compare and Branch

- `CMP rd, rs`
- `JMP addr`
- `JZ addr`
- `JNZ addr`
- `JC addr`
- `JNC addr`
- `JN addr`
- `CALL addr`
- `RET`

### System and IO

- `HLT` halt execution
- `IN rd, [addr]` read from memory mapped device
- `OUT rs, [addr]` write to memory mapped device

## Example Encoding Table

You can define the top 4 bits as the opcode:

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x0 | NOP | No operation |
| 0x1 | MOV | Register copy |
| 0x2 | LDI | Load immediate |
| 0x3 | LDR | Load from memory |
| 0x4 | STR | Store to memory |
| 0x5 | ADD | Add |
| 0x6 | SUB | Subtract |
| 0x7 | AND | Bitwise and |
| 0x8 | OR | Bitwise or |
| 0x9 | XOR | Bitwise xor |
| 0xA | CMP | Compare |
| 0xB | JMP and branches | Control flow |
| 0xC | PUSH and POP | Stack operations |
| 0xD | IN and OUT | IO operations |
| 0xE | CALL and RET | Subroutine control |
| 0xF | HLT | Halt |

## Addressing Modes

A small CPU benefits from only a few addressing modes.

- Immediate: constant embedded in instruction
- Register: operand stored in register
- Direct: instruction names a memory address
- Register indirect: register contains memory address
- Indexed: base register plus small offset
- Stack implied: operand uses `SP`

Examples:

```asm
LDI R0, 10        ; immediate
MOV R1, R0        ; register
LDR R2, [0x8000]  ; direct
LDR R3, [R1]      ; register indirect
STR R0, [R2+4]    ; indexed
PUSH R0           ; stack implied
```

## Memory Map

A simple 64 KB memory map:

| Address Range | Purpose |
|---------------|---------|
| `0x0000` to `0x1FFF` | ROM or program space |
| `0x2000` to `0xDFFF` | General RAM |
| `0xE000` to `0xE0FF` | Memory mapped IO |
| `0xE100` to `0xFFEF` | Extra RAM or reserved |
| `0xFFF0` to `0xFFFE` | Interrupt or system vectors |
| `0xFFFF` | Halt or reset flag location if desired |

### Example Memory Mapped IO Registers

| Address | Device |
|---------|--------|
| `0xE000` | Console output register |
| `0xE001` | Console input register |
| `0xE002` | Timer low byte |
| `0xE003` | Timer high byte |
| `0xE004` | Status register |

Writing an ASCII byte to `0xE000` prints a character to the host console.

## Emulator Design

The emulator models the behavior of the CPU and executes programs instruction by instruction.

### Core Responsibilities

- Initialize registers and memory
- Load binary program into memory
- Perform fetch, decode, execute cycle
- Update flags correctly
- Handle memory mapped IO reads and writes
- Support stepping, running, and memory dumping

### Main Classes

#### `CPU`

Responsible for the top level machine state.

Suggested fields:

```cpp
struct CPU {
    uint8_t r[4];
    uint16_t pc;
    uint16_t sp;
    uint16_t ir;
    uint8_t flags;
    bool halted;
};
```

#### `ALU`

Performs arithmetic and logic operations and returns flags.

Responsibilities:

- Add and subtract
- Bitwise operations
- Shift operations
- Flag calculations

#### `Memory`

Represents addressable memory.

Responsibilities:

- Byte reads and writes
- Bounds checking
- Loading binaries into ROM or RAM
- Dumping memory ranges

#### `Bus`

Provides a clean interface between CPU and memory or IO.

Responsibilities:

- Forward reads and writes
- Detect memory mapped IO addresses
- Route operations to device handlers

#### `ControlUnit`

Decodes opcodes and dispatches execution.

Responsibilities:

- Parse instruction fields
- Select ALU operation
- Trigger bus transactions
- Handle branches and stack logic

## Execution Cycle

Each instruction follows the classical cycle:

### 1. Fetch

- Read instruction word from memory at `PC`
- Store it in `IR`
- Increment `PC`

### 2. Decode and Compute

- Extract opcode and operands from `IR`
- Select the required ALU, branch, stack, or memory operation
- Compute result or effective address

### 3. Store and Commit

- Write result to destination register or memory
- Update flags
- Handle IO side effects

## Timer Example

A timer program is useful because it shows how repeated instruction cycles create visible behavior.

### Example Assembly

```asm
START:
    LDI R0, 0          ; counter
LOOP:
    OUT R0, [0xE002]   ; write low byte to timer display
    INC R0
    JMP LOOP
```

### How It Executes

#### Cycle 1

Fetch:
- CPU reads `LDI R0, 0`
- `PC` advances to next instruction

Compute:
- Control unit decodes `LDI`
- Immediate value `0` is selected

Store:
- `R0` becomes `0`
- Flags updated if your design does that for `LDI`

#### Cycle 2

Fetch:
- CPU reads `OUT R0, [0xE002]`

Compute:
- Control unit identifies an IO write
- Bus resolves `0xE002` as timer mapped device

Store:
- Value in `R0` is written to the timer device register
- Host emulator prints or stores updated timer state

#### Cycle 3

Fetch:
- CPU reads `INC R0`

Compute:
- ALU adds `1` to `R0`

Store:
- `R0` becomes `1`
- Flags updated

#### Cycle 4

Fetch:
- CPU reads `JMP LOOP`

Compute:
- Branch target is resolved

Store:
- `PC` is replaced with the address of `LOOP`

The cycle repeats, which causes the timer output to change every loop iteration.

## Assembler Design

The assembler converts human readable assembly into machine code.

### Responsibilities

- Tokenize source text
- Parse mnemonics and operands
- Resolve labels
- Parse numeric literals
- Encode machine instructions
- Output binary or hex files

### Recommended Two Pass Assembly

#### Pass 1

- Read source line by line
- Strip comments and whitespace
- Record label addresses
- Track instruction sizes

#### Pass 2

- Encode instructions using final label values
- Emit binary bytes
- Report syntax or range errors clearly

## Assembly Syntax

### Comments

Use semicolon comments.

```asm
LDI R0, 72    ; ASCII H
```

### Labels

Labels end with a colon.

```asm
LOOP:
    INC R0
    JMP LOOP
```

### Numeric Literals

Recommended forms:

- Decimal: `42`
- Hex: `0x2A`
- Binary: `0b101010`
- Character: `'A'`

### Example Parser Rules

- Mnemonics are case insensitive
- Registers are `R0` through `R3`
- Labels are alphanumeric with underscore
- Whitespace is ignored except as token separator

## Program Loading, Running, and Memory Dump

The emulator should provide a simple command line interface.

### Example Commands

```bash
./software_cpu assemble programs/hello.asm roms/hello.bin
./software_cpu run roms/hello.bin
./software_cpu dump 0x0000 0x0040
./software_cpu step roms/fibonacci.bin
```

### Suggested Runtime Features

- Run until `HLT`
- Step one instruction at a time
- Print registers after each step
- Dump memory ranges in hex
- Trace executed instructions
- Show writes to memory mapped IO

## Example Programs

### Hello, World

This example writes characters to the console output register.

```asm
    LDI R0, 'H'
    OUT R0, [0xE000]
    LDI R0, 'e'
    OUT R0, [0xE000]
    LDI R0, 'l'
    OUT R0, [0xE000]
    LDI R0, 'l'
    OUT R0, [0xE000]
    LDI R0, 'o'
    OUT R0, [0xE000]
    LDI R0, ','
    OUT R0, [0xE000]
    LDI R0, ' '
    OUT R0, [0xE000]
    LDI R0, 'W'
    OUT R0, [0xE000]
    LDI R0, 'o'
    OUT R0, [0xE000]
    LDI R0, 'r'
    OUT R0, [0xE000]
    LDI R0, 'l'
    OUT R0, [0xE000]
    LDI R0, 'd'
    OUT R0, [0xE000]
    LDI R0, '!'
    OUT R0, [0xE000]
    HLT
```

Expected output:

```text
Hello, World!
```

### Fibonacci Sequence

This example generates Fibonacci numbers iteratively.

```asm
    LDI R0, 0      ; a
    LDI R1, 1      ; b
    LDI R2, 10     ; count

LOOP:
    OUT R0, [0xE000]
    MOV R3, R0
    ADD R3, R1
    MOV R0, R1
    MOV R1, R3
    DEC R2
    JNZ LOOP
    HLT
```

In a more complete version, you may want to convert numeric values to ASCII before output if printing to a text console.

### Fibonacci Walkthrough

This program keeps two registers as the current pair of Fibonacci values.

- `R0` holds the current value `a`
- `R1` holds the next value `b`
- `R3` is a temporary register used to compute `a + b`
- `R2` counts how many numbers remain

Step by step:

1. Initialize `R0 = 0` and `R1 = 1`
2. Output the current Fibonacci value in `R0`
3. Compute next value into `R3`
4. Shift `R1` into `R0`
5. Shift `R3` into `R1`
6. Decrement counter in `R2`
7. Jump back until counter reaches zero

A sample sequence produced is:

```text
0 1 1 2 3 5 8 13 21 34
```

## Build Instructions

Example build using CMake:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Or directly with g++:

```bash
g++ -std=c++17 emulator.cpp isa.cpp assembler.cpp cpu.cpp -o software_cpu.exe
./software_cpu.exe
```

## Suggested Implementation Plan

### Phase 1

- Implement memory class
- Implement register state
- Implement fetch and decode loop
- Add `NOP`, `LDI`, `MOV`, `ADD`, `SUB`, `JMP`, `HLT`

### Phase 2

- Add flags and conditional branches
- Add load and store instructions
- Add stack operations
- Add memory dump and trace support

### Phase 3

- Add memory mapped IO
- Implement assembler
- Add example programs
- Add tests and documentation

## Testing

The project should include automated tests for:

- ALU operations and flag results
- Instruction decoding
- Branch behavior
- Memory reads and writes
- Assembler encoding correctness
- End to end execution of sample programs

Example test targets:

- `ADD` sets `Z` on zero result
- `SUB` sets `C` and `N` correctly
- `JZ` branches only when zero flag is set
- `hello.asm` produces `Hello, World!`
- `fibonacci.asm` produces the expected sequence

## Demo Video

The repository should include a demo video at `video/fibonacci_demo.mp4`.

The video should explain:

- The CPU architecture at a high level
- The Fibonacci assembly source
- What each register does in the program
- How the fetch, decode, execute cycle works
- How the Fibonacci values change over time
- The final output produced by the emulator

A good video flow is:

1. Show the CPU schematic
2. Open `fibonacci.asm`
3. Explain each instruction line by line
4. Run the assembler
5. Run the emulator in trace mode
6. Pause on a few cycles and explain register changes
7. Show final output and memory or register dump

## GitHub Repository Checklist

Before publishing, make sure the repository contains:

- Source code for the emulator
- Source code for the assembler
- Documentation in `README.md`
- CPU schematic image
- Example assembly programs
- Prebuilt binaries if required by the assignment
- Test files
- Demo video
- Clear build and run instructions

## Notes on Code Style

To keep the project easy to manage:

- Separate interfaces and implementation files
- Keep instruction decoding logic centralized
- Use descriptive names for opcodes and flags
- Add comments that explain why, not only what
- Prefer small functions for each instruction group
- Keep assembler parsing stages separate
- Add trace logging guarded by a debug flag

## Possible Extensions

Once the base CPU works, you can extend it with:

- More registers
- Wider instructions
- Interrupt support
- Call stack frames
- Better text and number output routines
- Simple graphics output via memory mapped framebuffer
- Disassembler support
- Breakpoints and interactive debugger

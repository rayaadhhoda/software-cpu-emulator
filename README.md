# Software CPU in C/C++

This project implements a small educational CPU in software. It includes a
custom instruction set, an assembler, an emulator, memory mapped IO, example
programs, and documentation for the CPU design.

The CPU is intentionally compact so the full machine can be understood from the
source code:

- 8-bit data registers
- 16-bit address space with 64 KB memory
- Fixed 4-byte instructions
- Four general purpose registers: `R0`, `R1`, `R2`, `R3`
- `PC`, `SP`, `IR`, and `FLAGS`
- ALU operations, control flow, stack operations, memory access, and IO

## Repository Layout

```text
software-cpu-emulator/
|-- CMakeLists.txt
|-- README.md
|-- emulator.cpp              # one-file unity build convenience entry
|-- include/                  # public headers
|-- src/                      # emulator, assembler, CPU, ALU, memory, bus
|-- programs/                 # example assembly programs
|-- docs/                     # schematic, ISA, memory map, execution cycle
|-- roms/                     # assembled binaries go here
|-- tests/                    # future automated tests
```

## CPU Design

The CPU has a register file, ALU, control unit, internal bus, memory, and memory
mapped IO devices.

See:

- [CPU schematic](docs/cpu_schematic.svg)
- [ISA](docs/isa.md)
- [Memory map](docs/memory_map.md)
- [Fetch/compute/store cycle](docs/execution_cycle.md)

## Build

With CMake:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Or compile directly with g++:

```bash
g++ -std=c++17 -Iinclude src/*.cpp -o software_cpu
```

There is also a unity build entry point for simple setups:

```bash
g++ -std=c++17 -Iinclude emulator.cpp -o software_cpu
```

On Windows PowerShell, the built executable will usually be run as
`.\software_cpu.exe`.

With Visual Studio Build Tools, open the **x64 Native Tools Command Prompt** and
run:

```bat
cl /std:c++17 /EHsc /Iinclude src\alu.cpp src\assembler.cpp src\bus.cpp src\cpu.cpp src\isa.cpp src\main.cpp src\memory.cpp /Fe:software_cpu.exe
```

## Usage

Assemble a program:

```bash
./software_cpu assemble programs/hello.asm roms/hello.bin
```

Run a program:

```bash
./software_cpu run roms/hello.bin
```

Run with instruction tracing:

```bash
./software_cpu run roms/fibonacci.bin --trace
```

Step through a fixed number of instructions:

```bash
./software_cpu step roms/fibonacci.bin --steps 8
```

Dump memory:

```bash
./software_cpu dump roms/fibonacci.bin 0x0000 0x003F
```

Run the timer demo with a cycle limit because it loops forever:

```bash
./software_cpu assemble programs/timer_demo.asm roms/timer_demo.bin
./software_cpu run roms/timer_demo.bin --max-cycles 20
```

## Example Programs

### Hello, World

[programs/hello.asm](programs/hello.asm) writes ASCII characters to memory
mapped address `0xE000`.

Expected output:

```text
Hello, World!
```

### Fibonacci Sequence

[programs/fibonacci.asm](programs/fibonacci.asm) prints the first ten Fibonacci
numbers using the numeric console port at `0xE001`.

Expected output:

```text
0 1 1 2 3 5 8 13 21 34
```

### Timer Demo

[programs/timer_demo.asm](programs/timer_demo.asm) repeatedly writes an
incrementing value to timer register `0xE002`. The execution cycle is explained
in [docs/execution_cycle.md](docs/execution_cycle.md).

## Memory Mapped IO

| Address | Behavior |
| --- | --- |
| `0xE000` | Print one ASCII character |
| `0xE001` | Print one unsigned decimal number and a space |
| `0xE002` | Store and display timer low byte |
| `0xE003` | Store timer high byte |
| `0xE004` | Status register |

## Project Status

Implemented:

- CPU state and execution loop
- ALU and flags
- Memory and bus
- Memory mapped console/timer IO
- Assembler with labels, numeric literals, and character literals
- CLI commands: `assemble`, `run`, `step`, and `dump`
- Hello World, Fibonacci, and timer example programs
- ISA, memory map, execution cycle docs, and schematic

Still to add before final submission:

- Automated tests
- Demo video for the Fibonacci program
- Final report document with team member names, GitHub link, run instructions,
  and contribution page
- Optional polish such as a disassembler or interactive debugger

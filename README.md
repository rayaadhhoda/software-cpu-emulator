# Software CPU Emulator

This branch contains a small C++ software CPU emulator that assembles and runs
assembly source files directly. The executable reads a `.asm` file, encodes the
instructions into an in-memory instruction array, and simulates them with a
simple fetch/decode/execute loop.

## Files

- `emulator.cpp` - command-line entry point.
- `assembler.cpp` / `assembler.h` - parses labels and encodes assembly lines.
- `simulate.cpp` / `simulate.h` - runs encoded instructions on the simulated CPU.
- `opcodes.cpp` / `opcodes.h` - opcode names and enum values.
- `dataFormat.h` - packed instruction/data representation.
- `register.h` - supported register names.
- `hello.asm` - prints `Hello, World!`.
- `fibonacci.asm` - prints Fibonacci values from `0` through `89`.
- `timer.asm` - prints integer ticks from `0` through `5`.

## Build

Compile the emulator from the C++ source files:

```powershell
g++ emulator.cpp assembler.cpp opcodes.cpp simulate.cpp -o cpu.exe
```

If you let `g++` use its default output name, the executable will be `a.exe`:

```powershell
g++ emulator.cpp assembler.cpp opcodes.cpp simulate.cpp
```

## Command-Line Usage

Run the emulator by passing an assembly file as the first argument:

```powershell
.\cpu.exe <program.asm>
```

The executable also supports a verbose simulation mode. In the current code, any
extra argument after the assembly filename enables verbose mode:

```powershell
.\cpu.exe <program.asm> verbose
```

`verbose` is used in the examples because it is readable, but the program only
checks whether one extra argument was provided.

## Running the Assembly Programs

Run `hello.asm`:

```powershell
.\cpu.exe hello.asm
```

Expected output:

```text
Hello, World!
```

Run `fibonacci.asm`:

```powershell
.\cpu.exe fibonacci.asm
```

Expected output:

```text
0
1
1
2
3
5
8
13
21
34
55
89
```

Run `timer.asm`:

```powershell
.\cpu.exe timer.asm
```

Expected output:

```text
0
1
2
3
4
5
```

Run `timer.asm` in verbose mode:

```powershell
.\cpu.exe timer.asm verbose
```

Verbose mode prints each cycle's fetch information, opcode, and store address in
addition to the program output. This is useful for watching the timer loop step
through `LDI`, `STR`, `ADDI`, `SLT`, and `BNZ` before it reaches `EXIT`.

## Assembly Format

Assembly files use `.global` and `.text` sections. Only instructions in `.text`
are encoded and executed. Labels end with `:` and can be used as branch or call
targets.

Example:

```asm
.global

.text
    LDI %a, #0
LOOP:
    LDI %b, #12288
    STR %a, %b [0]
    ADDI %a, #1
    SLT %b, %a, #6
    BNZ %b, LOOP
    EXIT
```

Supported registers in this branch:

- `%a`
- `%b`
- `%sp`
- `%ra`

Other register enum values exist in `register.h`, but the assembler currently
parses only the names listed above.

## Implemented Instructions

The opcode table contains more instructions than the simulator currently
executes. The implemented simulator cases are:

- `ADD`
- `LDI`
- `ADDI`
- `SUBI`
- `MOV`
- `PUSH`
- `POP`
- `STR`
- `EXIT`
- `SLT`
- `BZ`
- `BNZ`
- `CALL`
- `RET`

## Output Addresses

`STR` has two memory-mapped output addresses:

- `12288` - prints the destination register as an integer followed by a newline.
- `12289` - prints the destination register as a character.

For example, `hello.asm` stores ASCII values to `12289`, while `fibonacci.asm`
and `timer.asm` store integers to `12288`.

## Current Limits

- Instruction memory is fixed at 1024 instructions.
- The simulated stack/data buffer is fixed at 1024 `Data` entries.
- General memory writes are not implemented yet; `STR` is currently used for
  memory-mapped output.
- Verbose mode is enabled by argument count rather than by checking a specific
  flag name.

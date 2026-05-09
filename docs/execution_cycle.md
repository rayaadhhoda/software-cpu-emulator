# Fetch, Compute, Store Cycle

Every instruction runs through three conceptual stages.

## 1. Fetch

The CPU reads four bytes from memory at `PC`, combines them into `IR`, and then
advances `PC` by four bytes.

## 2. Compute

The control unit decodes the opcode and operands. It selects the ALU operation,
computes a memory address, checks branch flags, or prepares an IO operation.

## 3. Store

The CPU commits the result by writing a register, memory address, IO register,
stack entry, or program counter. Instructions that produce arithmetic or logic
results also update flags.

## Timer Example

Source:

```asm
    LDI R0, 0
LOOP:
    OUT R0, [0xE002]
    INC R0
    JMP LOOP
```

Cycle 1:

- Fetch: read `LDI R0, 0`
- Compute: select the immediate value `0`
- Store: write `0` into `R0`, set `Z`, clear `N`

Cycle 2:

- Fetch: read `OUT R0, [0xE002]`
- Compute: resolve `0xE002` as the timer IO register
- Store: write the current value of `R0` to the timer display

Cycle 3:

- Fetch: read `INC R0`
- Compute: ALU adds one to `R0`
- Store: write the incremented value back to `R0` and update flags

Cycle 4:

- Fetch: read `JMP LOOP`
- Compute: resolve `LOOP` to its instruction address
- Store: replace `PC` with the loop address

The program repeats forever unless the emulator is run with a cycle limit.

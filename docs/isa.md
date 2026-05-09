# Instruction Set Architecture

This CPU is an 8-bit teaching processor with 16-bit addressing. It uses four
general purpose registers, a program counter, a stack pointer, an instruction
register, and a flags register.

## Registers

| Register | Size | Purpose |
| --- | ---: | --- |
| R0 | 8 bit | General purpose |
| R1 | 8 bit | General purpose |
| R2 | 8 bit | General purpose |
| R3 | 8 bit | General purpose |
| PC | 16 bit | Program counter |
| SP | 16 bit | Stack pointer |
| IR | 32 bit | Current instruction |
| FLAGS | 8 bit | Z, C, N, V condition flags |

## Instruction Format

Instructions are fixed width 32-bit values stored little endian in memory.

```text
31          16 15     12 11      8 7          0
+-------------+---------+---------+------------+
| operand16   |   rs    |   rd    | opcode     |
+-------------+---------+---------+------------+
```

- `opcode`: instruction selector
- `rd`: destination register or single register operand
- `rs`: source register for register-register instructions
- `operand16`: immediate value, memory address, or branch target

The CPU still has an 8-bit data path. The 16-bit operand field exists so memory
and branches can address the full 64 KB memory space.

## Encoding

| Opcode | Mnemonic | Operands | Description |
| ---: | --- | --- | --- |
| 0x00 | NOP | none | No operation |
| 0x01 | MOV | rd, rs | Copy register |
| 0x02 | LDI | rd, imm8 | Load 8-bit immediate |
| 0x03 | LDR | rd, [addr16] | Load byte from memory |
| 0x04 | STR | rs, [addr16] | Store byte to memory |
| 0x05 | ADD | rd, rs | Add into rd |
| 0x06 | SUB | rd, rs | Subtract rs from rd |
| 0x07 | INC | rd | Increment register |
| 0x08 | DEC | rd | Decrement register |
| 0x09 | AND | rd, rs | Bitwise and |
| 0x0A | OR | rd, rs | Bitwise or |
| 0x0B | XOR | rd, rs | Bitwise xor |
| 0x0C | NOT | rd | Bitwise not |
| 0x0D | SHL | rd | Shift left |
| 0x0E | SHR | rd | Shift right |
| 0x0F | CMP | rd, rs | Compare by subtracting without storing |
| 0x10 | JMP | addr16 | Unconditional jump |
| 0x11 | JZ | addr16 | Jump if zero flag set |
| 0x12 | JNZ | addr16 | Jump if zero flag clear |
| 0x13 | JC | addr16 | Jump if carry flag set |
| 0x14 | JNC | addr16 | Jump if carry flag clear |
| 0x15 | PUSH | rs | Push register on stack |
| 0x16 | POP | rd | Pop stack into register |
| 0x17 | CALL | addr16 | Push return address and jump |
| 0x18 | RET | none | Return from subroutine |
| 0x19 | IN | rd, [addr16] | Read memory mapped IO |
| 0x1A | OUT | rs, [addr16] | Write memory mapped IO |
| 0xFF | HLT | none | Halt execution |

## Addressing Modes

| Mode | Example | Meaning |
| --- | --- | --- |
| Register | `ADD R0, R1` | Operand is held in a register |
| Immediate | `LDI R0, 42` | Constant is encoded in the instruction |
| Direct | `LDR R0, [0x2000]` | Operand is read from memory |
| Branch direct | `JNZ LOOP` | Label resolves to an instruction address |
| Stack implied | `PUSH R0` | Uses the stack pointer |

## Flag Semantics

| Flag | Meaning |
| --- | --- |
| Z | Result was zero |
| C | Addition carried out, subtraction borrowed, or shift lost a bit |
| N | Result bit 7 was set |
| V | Signed overflow occurred |

`ADD`, `SUB`, `INC`, `DEC`, `CMP`, and bitwise operations update flags. `MOV`,
`LDI`, `LDR`, `IN`, and `POP` update `Z` and `N`. Stores, output, branches, and
stack pushes leave flags unchanged.

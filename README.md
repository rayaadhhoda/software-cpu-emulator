## System Design and Execution Model

The Software CPU Emulator is designed with a modular and extensible architecture that separates key subsystems including the Instruction Set Architecture (ISA), CPU, Memory, Assembler, and Execution Engine.

Execution follows a standard fetch-decode-execute cycle:
1. Fetch instruction from memory using the Program Counter (PC)
2. Decode opcode and operands
3. Execute operation and update registers/memory
4. Advance PC or branch as needed

### Function Calls and Stack Behavior

The CPU supports function calls and recursion using a stack-based mechanism:

- `CALL` stores the return address in the return address register (`ra`) and jumps to the target label
- `RET` restores execution by jumping back to the saved return address
- `PUSH` and `POP` allow saving/restoring register values

This enables recursive programs such as Fibonacci or factorial (with proper base cases).

### Memory Model (Design + Current Implementation)

The architecture is designed around a 64 KB memory space with separation of concerns:

| Region | Purpose |
|------|--------|
| Program Space (ROM) | Stores instructions |
| Data/Stack Space (RAM) | Runtime data and stack |
| Memory-Mapped IO | Device interaction |

In the current implementation, a simplified memory model is used:

- Instruction memory is stored in an internal instruction array
- Stack/data memory is emulated using a fixed-size buffer
- Memory-mapped I/O is implemented via special addresses:
  - `12288` → character output
  - `12289` → integer output

This design allows different programs (e.g., `hello.asm` vs `fibonacci.asm`) to use the same CPU with different output behaviors.

### Registers

The CPU includes:
- General-purpose registers (`%a`, `%b`, etc.)
- Program Counter (`pc`)
- Stack Pointer (`sp`)
- Return Address (`ra`)
- Internal flag register (used for branching)

---

### Design Philosophy

The emulator prioritizes:
- Clarity of instruction execution
- Ease of extending the ISA
- Separation between parsing, execution, and memory handling

The current implementation represents a working subset of the full architecture, with clear paths for extension such as:
- Full 64 KB memory backing
- Binary loading instead of direct assembly parsing
- Additional instructions and flag handling
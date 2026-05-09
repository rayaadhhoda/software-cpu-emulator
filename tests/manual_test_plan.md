# Manual Test Plan

These are the first checks to run after a C++ compiler is available.

```bash
g++ -std=c++17 -Iinclude src/*.cpp -o software_cpu

./software_cpu assemble programs/hello.asm roms/hello.bin
./software_cpu run roms/hello.bin

./software_cpu assemble programs/fibonacci.asm roms/fibonacci.bin
./software_cpu run roms/fibonacci.bin

./software_cpu step roms/fibonacci.bin --steps 8
./software_cpu dump roms/fibonacci.bin 0x0000 0x003F

./software_cpu assemble programs/timer_demo.asm roms/timer_demo.bin
./software_cpu run roms/timer_demo.bin --max-cycles 12
```

Expected results:

- Hello prints `Hello, World!`
- Fibonacci prints `0 1 1 2 3 5 8 13 21 34`
- Step mode prints traced instructions and register values
- Dump mode prints assembled bytes from memory
- Timer mode prints incrementing timer values until the cycle limit stops it

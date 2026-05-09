; Writes "Hello, World!" to the memory mapped console.

.text
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

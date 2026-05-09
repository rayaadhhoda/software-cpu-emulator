; Prints the first ten Fibonacci numbers through the numeric console port.
; R0 = current value
; R1 = next value
; R2 = remaining count
; R3 = temporary sum

.text
    LDI R0, 0
    LDI R1, 1
    LDI R2, 10

LOOP:
    OUT R0, [0xE001]
    MOV R3, R0
    ADD R3, R1
    MOV R0, R1
    MOV R1, R3
    DEC R2
    JNZ LOOP
    HLT

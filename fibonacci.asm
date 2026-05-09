.global

.text
    LDI %a, #0

LOOP:
    PUSH %a
    CALL factorial
    POP %a
    LDI %b, #12288
    STR %a, %b[0]
    ADDI %a, #1
    BNE %a, #12, LOOP
    EXIT

factorial:
    PUSH %ra
    SLT %a, %a, #2
    BEQ RETURN
    PUSH %a
    CALL factorial
    POP %b
    PUSH %a
    MOV %a, %b
    CALL factorial
    POP %b
    ADD %a, %b
RETURN:
    POP %ra
    RET
    
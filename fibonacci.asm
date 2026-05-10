.global

.text
    LDI %a, #0
LOOP:
    PUSH %a
    CALL factorial
    LDI %b, #12289
    STR %a, %b [0]
    POP %a
    ADDI %a, #1
    SLT %b, %a, #12
    BNZ %b, LOOP
    EXIT

factorial:
    PUSH %ra
    SLT %b, %a, #2
    BNZ %b, RETURN
    PUSH %a
    SUBI %a, #1
    CALL factorial
    POP %b
    PUSH %a
    MOV %a, %b
    SUBI %a, #2
    CALL factorial
    POP %b
    ADD %a, %b
RETURN:
    POP %ra
    RET

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

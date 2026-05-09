; Infinite timer demo. Run it with a cycle limit:
; software_cpu run roms/timer_demo.bin --max-cycles 20

.text
    LDI R0, 0

LOOP:
    OUT R0, [0xE002]
    INC R0
    JMP LOOP

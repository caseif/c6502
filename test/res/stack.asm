;;;;;;;;;;;;;;;;
; test stack operations
;;;;;;;;;;;;;;;;

.org $8000

;;;;;;;;;;;;;;;;
; basic stack operations
;;;;;;;;;;;;;;;;

LDX #$FF            ; quick hack to initialize the stack pointer as 0xFF, since
TXS                 ; CPU init starts with an RST which decrements the stack
                    ; pointer twice when it pushes the address

LDA #$01            ; set acc
PHA                 ; push acc to stack
LDA #$02            ; set acc
PHA                 ; push acc to stack

LDA #$00            ; reset acc

TSX                 ; copy sp to x
PLA                 ; pull acc from stack
TAY                 ; copy acc to y
PLA                 ; pull acc from stack

NOP                 ; perform assertions:
                    ; a = 0x01
                    ; x = 0xFD
                    ; y = 0x02
                    ; sp = 0xFF

LDA #$01            ; set acc
PHA                 ; push acc to stack
LDA #$02            ; set acc
PHA                 ; push acc to stack

LDX #$FC            ; set x=0xFC
TXS                 ; set sp=0xFC

PLA                 ; pull acc from stack
TAX                 ; copy acc to x
PLA                 ; pull acc from stack
TAY                 ; copy acc to y
PLA                 ; pull acc from stack

NOP                 ; perform assertions:
                    ; a = 0x01
                    ; x = 0x00
                    ; y = 0x02
                    ; sp = 0xFF

;;;;;;;;;;;;;;;;
; processor status push/pop
;;;;;;;;;;;;;;;;

SEC                 ; set carry flag
SEI                 ; set interrupt disable just for fun
PHP                 ; push status to stack

CLC                 ; clear carry flag
CLI                 ; clear interrupt disable flag

PLP                 ; pull status from stack

NOP                 ; perform assertions:
                    ; C = 1
                    ; Z = 0
                    ; I = 1
                    ; N = 0

.org $BFFA
.dw $8000
.dw $8000
.dw $8000

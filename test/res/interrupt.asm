;;;;;;;;;;;;;;;;
; test interrupts
;;;;;;;;;;;;;;;;

; required for execution on the NES
.org $8000

SEC                 ; set carry bit
BRK                 ; issue software interrupt
LDA #$01            ; set A = 1
LDY #$01            ; set Y = 1
NOP                 ; perform assertions:
                    ;     A = 1
                    ;     C = 1

handler:
CLC                 ; clear carry bit
LDX #$01            ; set X = 1
RTI                 ; return from interrupt

.org $BFFE          ; constant location of interrupt vector (we take advantage of mirroring here)
.dw handler         ; interrupt vector itself

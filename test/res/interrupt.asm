;;;;;;;;;;;;;;;;
; test interrupts
;;;;;;;;;;;;;;;;

.org $8000

SEC                 ; set carry bit

BRK                 ; issue software interrupt
;TODO: Mossy changed to add a padding byte after BRK,
;not sure if that should be reverted
;CLC                 ; clear carry bit - should be skipped!
LDA #$01            ; set A = 1
LDY #$01            ; set Y = 1
NOP                 ; perform assertions:
                    ;     A = 0
                    ;     X = 1
                    ;     Y = 1
                    ;     C = 1

handler:
CLC                 ; clear carry bit
LDX #$01            ; set X = 1
RTI                 ; return from interrupt

.org $BFFA
.dw $8000
.dw $8000
.dw handler         ; special interrupt handler

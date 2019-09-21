;;;;;;;;;;;;;;;;
; test subtraction
;;;;;;;;;;;;;;;;

; required for execution on the NES
.org $8000

SEC                 ; set carry flag
LDA #$20            ; init acc
SBC #$10            ; subtract 0x10
NOP                 ; perform assertions:
                    ; a = 0x10
                    ; c = 1
                    ; n = 0
                    ; v = 0
                    ; z = 0

CLC                 ; clear carry flag
LDA #$20            ; init acc
SBC #$10            ; subtract 0x10
NOP                 ; perform assertions:
                    ; a = 0x0F
                    ; c = 1
                    ; n = 0
                    ; v = 0
                    ; z = 0

SEC                 ; set carry flag
LDA #$20            ; init acc
SBC #$20            ; subtract 0x20
NOP                 ; perform assertions:
                    ; a = 0
                    ; c = 1
                    ; n = 0
                    ; v = 0
                    ; z = 1

SEC                 ; set carry flag
LDA #$20            ; init acc
SBC #$30            ; subtract 0x30
NOP                 ; perform assertions:
                    ; a = 0xF0
                    ; c = 0
                    ; n = 1
                    ; v = 0
                    ; z = 0

SEC                 ; set carry flag
LDA #$20            ; init acc
SBC #$80            ; subtract 0x80
NOP                 ; perform assertions
                    ; a = 0xA0
                    ; c = 0
                    ; n = 1
                    ; v = 1
                    ; z = 0

SEC                 ; set carry flag
LDA #$80            ; init acc
SBC #$20            ; subtract 0x20
NOP                 ; perform assertions
                    ; a = 0x60
                    ; c = 1
                    ; n = 0
                    ; v = 1
                    ; z = 0

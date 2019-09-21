;;;;;;;;;;;;;;;;
; test bitwise logic
;;;;;;;;;;;;;;;;

; required for execution on the NES
.org $8000

JMP entry_point     ; skip to entry point

; small convenience subroutine for left-shifting memory contents
set_and_asl:
STA $00             ; write acc to first byte of memory
ASL $00             ; shift first byte of memory left, should produce 0b00001110
LDA $00             ; load byte back into acc
RTS

entry_point:

;;;;;;;;;;;;;;;;
; test AND
;;;;;;;;;;;;;;;;

LDA #%00011100      ; set accumulator
AND #%00101110      ; logical and, should produce 0b00001100
NOP                 ; perform assertions:
                    ; a = 12
                    ; n = 0
                    ; z = 0

LDA #%11111111      ; set accumulator
AND #%10000000      ; logical and, should produce 0b10000000
NOP                 ; perform assertions:
                    ; a = 0x80
                    ; n = 1
                    ; z = 0

LDA #%11110000      ; set accumulator
AND #%00001111      ; logical and, should produce 0
NOP                 ; perform assertions:
                    ; a = 0
                    ; n = 0
                    ; z = 1

;;;;;;;;;;;;;;;;
; test EOR
;;;;;;;;;;;;;;;;

LDA #%00110011      ; set accumulator
EOR #%00111111      ; logical xor, should produce 0b00001100
NOP                 ; perform assertions:
                    ; a = 12
                    ; n = 0
                    ; z = 0

LDA #%00110011      ; set accumulator
EOR #%11111111      ; logical xor, should produce 0b11001100
NOP                 ; perform assertions:
                    ; a = 0xCC
                    ; n = 1
                    ; z = 0

LDA #%00110011      ; set accumulator
EOR #%00110011      ; logical xor, should produce 0
NOP                 ; perform assertions:
                    ; a = 12
                    ; n = 0
                    ; z = 1

;;;;;;;;;;;;;;;;
; test ORA
;;;;;;;;;;;;;;;;

LDA #%00110011      ; set accumulator
ORA #%00001111      ; logical or, should produce 0b00111111
NOP                 ; perform assertions:
                    ; a = 0x5F
                    ; n = 0
                    ; z = 0

LDA #%11001100      ; set accumulator
ORA #%00110011      ; logical or, should produce 0b11111111
NOP                 ; perform assertions:
                    ; a = 0xFF
                    ; n = 1
                    ; z = 0

LDA #%00000000      ; set accumulator
ORA #%00000000      ; logical or, should produce 0
NOP                 ; perform assertions:
                    ; a = 0
                    ; n = 0
                    ; z = 1

;;;;;;;;;;;;;;;;
; test ASL
;;;;;;;;;;;;;;;;

LDA #%00000111      ; set accumulator
ASL                 ; shift left, should produce 0b00001110
NOP                 ; perform assertions:
                    ; a = 14
                    ; c = 0
                    ; n = 0
                    ; z = 0

LDA #%01111111      ; set accumulator
ASL                 ; shift left, should produce 0b11111110
NOP                 ; perform assertions:
                    ; a = 0xFE
                    ; c = 0
                    ; n = 1
                    ; z = 0

LDA #%10000001      ; set accumulator
ASL                 ; shift left, should produce 0b00000010
NOP                 ; perform assertions:
                    ; a = 2
                    ; c = 1
                    ; n = 0
                    ; z = 0

LDA #%10000000      ; set accumulator
ASL                 ; shift left, should produce 0b00000000
NOP                 ; perform assertions:
                    ; a = 0
                    ; c = 1
                    ; n = 0
                    ; z = 1

;;;;;;;;;;;;;;;;
; test ASL (memory)
;;;;;;;;;;;;;;;;

LDA #%00000111      ; set accumulator
JSR set_and_asl     ; left-shift memory byte
NOP                 ; perform assertions:
                    ; a = 14
                    ; c = 0
                    ; n = 0
                    ; z = 0

LDA #%01111111      ; set accumulator
JSR set_and_asl     ; left-shift memory byte
NOP                 ; perform assertions:
                    ; a = 0xFE
                    ; c = 0
                    ; n = 1
                    ; z = 0

LDA #%10000000      ; set accumulator
JSR set_and_asl     ; left-shift memory byte
NOP                 ; perform assertions:
                    ; a = 0
                    ; c = 1
                    ; n = 0
                    ; z = 1

;;;;;;;;;;;;;;;;
; test LSR
;;;;;;;;;;;;;;;;

LDA #%00000110      ; set accumulator
LSR                 ; shift right, should produce 0b00000011
NOP                 ; perform assertions:
                    ; a = 3
                    ; c = 0
                    ; n = 0
                    ; z = 0

LDA #%00000111      ; set accumulator
LSR                 ; shift right, should produce 0b00000011
NOP                 ; perform assertions:
                    ; a = 3
                    ; c = 1
                    ; n = 0
                    ; z = 0

LDA #%00000001      ; set accumulator
LSR                 ; shift right, should produce 0b00000000
NOP                 ; perform assertions:
                    ; a = 0
                    ; c = 1
                    ; n = 0
                    ; z = 1

;;;;;;;;;;;;;;;;
; test ROL
;;;;;;;;;;;;;;;;

CLC                 ; clear the carry flag
LDA #%00000111      ; set accumulator
ROL                 ; rotate left, should produce 0b00001110
NOP                 ; perform assertions:
                    ; a = 14
                    ; c = 0
                    ; n = 0
                    ; z = 0

SEC                 ; set the carry flag
LDA #%00000111      ; set accumulator
ROL                 ; shift left, should produce 0b00001111
NOP                 ; perform assertions:
                    ; a = 15
                    ; c = 0
                    ; n = 0
                    ; z = 0

SEC                 ; set the carry flag
LDA #%01111111      ; set accumulator
ROL                 ; shift left, should produce 0b11111111
NOP                 ; perform assertions:
                    ; a = 0xFF
                    ; c = 0
                    ; n = 1
                    ; z = 0

SEC                 ; set the carry flag
LDA #%10000001      ; set accumulator
ROL                 ; shift left, should produce 0b00000011
NOP                 ; perform assertions:
                    ; a = 3
                    ; c = 1
                    ; n = 0
                    ; z = 0

CLC                 ; clear the carry flag
LDA #%10000000      ; set accumulator
ROL                 ; shift left, should produce 0b00000000
NOP                 ; perform assertions:
                    ; a = 0
                    ; c = 1
                    ; n = 0
                    ; z = 1

;;;;;;;;;;;;;;;;
; test ROR
;;;;;;;;;;;;;;;;

CLC                 ; clear the carry flag
LDA #%00000110      ; set accumulator
ROR                 ; shift right, should produce 0b00000011
NOP                 ; perform assertions:
                    ; a = 3
                    ; c = 0
                    ; n = 0
                    ; z = 0

SEC                 ; set the carry flag
LDA #%00000110      ; set accumulator
ROR                 ; shift right, should produce 0b10000011
NOP                 ; perform assertions:
                    ; a = 0x83
                    ; c = 0
                    ; n = 1
                    ; z = 0

SEC                 ; set the carry flag
LDA #%00000111      ; set accumulator
ROR                 ; shift right, should produce 0b10000011
NOP                 ; perform assertions:
                    ; a = 0x83
                    ; c = 1
                    ; n = 1
                    ; z = 0

CLC                 ; clear the carry flag
LDA #%00000001      ; set accumulator
ROR                 ; shift right, should produce 0b00000000
NOP                 ; perform assertions:
                    ; a = 0
                    ; c = 1
                    ; n = 0
                    ; z = 1

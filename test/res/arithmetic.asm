;;;;;;;;;;;;;;;;
; test basic arithmetic
;;;;;;;;;;;;;;;;

; required for execution on the NES
.org $8000

; skip to the entry point
JMP entry_point

; small subroutune for conveience. it increments the first byte of memory,
; then copies it to the accumulator
inc_mem:
INC $00             ; increment memory value
LDA $00             ; copy result to acc
RTS                 ; return

; same as inc_mem, but for decrementing
dec_mem:
DEC $00             ; decrement memory value
LDA $00             ; copy result to acc
RTS                 ; return

; small subroutine for convenience. it does the following things:
; - write the accumulator to memory
; - increments it once
; - copies the result back to the accumulator
set_and_inc_mem:
STA $00             ; write acc to memory
JSR inc_mem         ; call subroutine for incrementing
RTS                 ; return

; same as the set_and_inc_mem, but for decrementing
set_and_dec_mem:
STA $00             ; write acc to memory
JSR dec_mem         ; call subroutine for decrementing
RTS                 ; return

entry_point:

;;;;;;;;;;;;;;;;
; test INX
;;;;;;;;;;;;;;;;

LDX #$00            ; set x=0
INX                 ; increment by 1
NOP                 ; perform assertions:
                    ; x = 1
                    ; n = 0
                    ; z = 0

LDX #$FF            ; set x=0xFF
INX                 ; increment by 1
NOP                 ; perform assertions:
                    ; x = 0
                    ; n = 0
                    ; z = 1

LDX #$7F            ; set x=0x7F
INX                 ; increment by 1
NOP                 ; perform assertions:
                    ; x = 0x80
                    ; n = 1
                    ; z = 0

LDX #$80            ; set x=0x80
INX                 ; increment by 1
NOP                 ; perform assertions:
                    ; x = 0x81
                    ; n = 1
                    ; z = 0

;;;;;;;;;;;;;;;;
; test INY
;;;;;;;;;;;;;;;;

LDY #$00            ; set y=0
INY                 ; increment by 1
NOP                 ; perform assertions:
                    ; y = 1
                    ; n = 0
                    ; z = 0

LDY #$FF            ; set y=0xFF
INY                 ; increment by 1
NOP                 ; perform assertions:
                    ; y = 0
                    ; n = 0
                    ; z = 1

LDY #$7F            ; set y=0x7F
INY                 ; increment by 1
NOP                 ; perform assertions:
                    ; y = 0x80
                    ; n = 1
                    ; z = 0

LDY #$80            ; set y=0x80
INY                 ; increment by 1
NOP                 ; perform assertions:
                    ; y = 0x81
                    ; n = 1
                    ; z = 0

;;;;;;;;;;;;;;;;
; test INC
;;;;;;;;;;;;;;;;

LDA #$00            ; set x=0
JSR set_and_inc_mem ; increment the memory value
NOP                 ; perform assertions:
                    ; a = 1
                    ; n = 0
                    ; z = 0

LDA #$FF            ; set a=0xFF
JSR set_and_inc_mem ; increment the memory value
NOP                 ; perform assertions:
                    ; a = 0
                    ; n = 0
                    ; z = 1

LDA #$7F            ; set a=0x7F
JSR set_and_inc_mem ; increment the memory value
NOP                 ; perform assertions:
                    ; a = 0x80
                    ; n = 1
                    ; z = 0

LDA #$80            ; set a=0x80
JSR set_and_inc_mem ; increment the memory value
NOP                 ; perform assertions:
                    ; a = 0x81
                    ; n = 1
                    ; z = 0

;;;;;;;;;;;;;;;;
; test DEX
;;;;;;;;;;;;;;;;

LDX #$02            ; set x=2
DEX                 ; decrement by 1
NOP                 ; perform assertions:
                    ; x = 1
                    ; n = 0
                    ; z = 0

DEX                 ; decrement again
NOP                 ; perform assertions:
                    ; x = 0
                    ; n = 0
                    ; z = 1

DEX                 ; decrement again
NOP                 ; perform assertions:
                    ; x = 0xFF
                    ; n = 1
                    ; z = 0

LDX #$80            ; set x=0x80
DEX                 ; decrement by 1
NOP                 ; perform assertions:
                    ; x = 0x7F
                    ; n = 0
                    ; z = 0

;;;;;;;;;;;;;;;;
; test DEY
;;;;;;;;;;;;;;;;

LDY #$02            ; set y=2
DEY                 ; decrement by 1
NOP                 ; perform assertions:
                    ; y = 1
                    ; n = 0
                    ; z = 0

DEY                 ; decrement again
NOP                 ; perform assertions:
                    ; y = 0
                    ; n = 0
                    ; z = 1

DEY                 ; decrement again
NOP                 ; perform assertions:
                    ; y = 0xFF
                    ; n = 1
                    ; z = 0

LDY #$80            ; set y=0x80
DEY                 ; decrement by 1
NOP                 ; perform assertions:
                    ; y = 0x7F
                    ; n = 0
                    ; z = 0

;;;;;;;;;;;;;;;;
; test DEC
;;;;;;;;;;;;;;;;

LDA #$02            ; set a=2
JSR set_and_dec_mem ; decrement by 1
NOP                 ; perform assertions:
                    ; y = 1
                    ; n = 0
                    ; z = 0

JSR dec_mem         ; decrement again
NOP                 ; perform assertions:
                    ; y = 0
                    ; n = 0
                    ; z = 1

JSR dec_mem         ; decrement again
NOP                 ; perform assertions:
                    ; y = 0xFF
                    ; n = 1
                    ; z = 0

LDA #$80            ; set a=0x80
JSR set_and_dec_mem ; decrement by 1
NOP                 ; perform assertions:
                    ; y = 0x7F
                    ; n = 0
                    ; z = 0

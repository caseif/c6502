;;;;;;;;;;;;;;;;
; test cpu status
;;;;;;;;;;;;;;;;

; required for execution on the NES
.org $8000

;;;;;;;;;;;;;;;;
; test explicit flag-setting
;;;;;;;;;;;;;;;;

SEC                 ; set carry flag
SEI                 ; set interrupt-disable flag
NOP                 ; perform assertions:
                    ; c = 1
                    ; i = 1

; since we can't set the overflow flag explicitly,
; we rely on the test supervisor to do it for us
; at this point so we can test _all_ of the clear
; instructions

;;;;;;;;;;;;;;;;
; test explicit flag-clearing
;;;;;;;;;;;;;;;;

CLC                 ; clear carry flag
CLI                 ; clear interrupt-disable flag
CLV                 ; clear overflow flag
NOP                 ; perform assertions:
                    ; c = 0
                    ; i = 0
                    ; v = 0

;;;;;;;;;;;;;;;;
; test comparisons
;;;;;;;;;;;;;;;;

; test CMP

LDA #$01            ; set a=1
CMP #$01            ; a == 1
NOP                 ; perform assertions:
                    ; c = 1
                    ; n = 0
                    ; z = 1

CMP #$00            ; a > 0
NOP                 ; perform assertions:
                    ; c = 1
                    ; n = 0
                    ; z = 0

CMP #$02            ; a < 2
NOP                 ; perform assertions:
                    ; c = 0
                    ; n = 1
                    ; z = 0

LDA #$81            ; set a=0x81 (-127)
CMP #$80            ; a > 0x80
NOP                 ; perform assertions:
                    ; c = 1
                    ; n = 0
                    ; z = 0

CMP #$01            ; a > 1 (when unsigned)
NOP                 ; perform assertions:
                    ; c = 1
                    ; n = 1
                    ; z = 0

CMP #$82            ; a < 0x82
NOP                 ; perform assertions:
                    ; c = 0
                    ; n = 1
                    ; z = 0

; CMP shares its implementation with CPX and CPY,
; so as long as those instructions work at all,
; we should be good to go

; test CPX

LDX #$01            ; set x=1
CPX #$01            ; x == 1
NOP                 ; perform assertions:
                    ; c = 1
                    ; n = 0
                    ; z = 1

LDY #$01            ; set y=1
CPY #$01            ; y == 1
NOP                 ; perform assertions:
                    ; c = 1
                    ; n = 0
                    ; z = 1

;;;;;;;;;;;;;;;;
; test addition
;;;;;;;;;;;;;;;;

; required for execution on the NES
.org $8000

;;;;;;;;;;;;;;;;
; without carry
;;;;;;;;;;;;;;;;

CLC             ; clear carry bit
LDA #$01        ; init acc
ADC #$01        ; add 1 to acc
NOP             ; perform assertions:
                ; A = 0x02
                ; C = 0
                ; Z = 0
                ; V = 0
                ; N = 0

CLC             ; clear carry bit
LDA #$FE        ; init acc
ADC #$03        ; add 3 to acc, overflowing it
NOP             ; perform assertions:
                ; A = 0x01
                ; C = 1
                ; Z = 0
                ; V = 0
                ; N = 0

CLC             ; clear carry bit
LDA #$40        ; init acc
ADC #$40        ; add 64 to acc, overflowing the signed result
NOP             ; perform assertions:
                ; A = 0x80
                ; C = 0
                ; Z = 0
                ; V = 1
                ; N = 1

CLC             ; clear carry bit
LDA #$80        ; init acc
ADC #$80        ; add 128/-128, overflowing the accumulator to 0
NOP             ; perform assertions:
                ; A = 0x00
                ; C = 1
                ; Z = 1
                ; V = 1
                ; N = 0

;;;;;;;;;;;;;;;;
; with carry
;;;;;;;;;;;;;;;;

SEC             ; set carry bit
LDA #$00        ; init acc
ADC #$01        ; add 1 to acc
NOP             ; perform assertions:
                ; A = 0x02
                ; C = 0
                ; Z = 0
                ; V = 0
                ; N = 0

SEC             ; set carry bit
LDA #$7E        ; init acc
ADC #$01        ; add 1 to acc, overflowing the signed result
NOP             ; perform assertions:
                ; A = 0x80
                ; C = 0
                ; Z = 0
                ; V = 1
                ; N = 1

SEC             ; set carry bit
LDA #$FE        ; init acc
ADC #$01        ; add 1 to acc, overflowing the unsigned result
NOP             ; perform assertions:
                ; A = 0x00
                ; C = 1
                ; Z = 1
                ; V = 0
                ; N = 0

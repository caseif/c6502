;;;;;;;;;;;;;;;;
;;; store/load tests
;;;;;;;;;;;;;;;;

; required for execution on the NES
.org $8000

;;;;;;;;;;;;;;;;
;; accumulator tests
;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;
; test zero-page addressing
;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;
; test storing
;;;;;;;;;;;;;;;;

LDA #$01            ; a = 0x01
STA $10             ; write a=1 to $0010
STA $90             ; write a=1 to $0090
STA $FF             ; write a=1 to $00FF
NOP                 ; perform assertions:
                    ; $0010 = 0x01
                    ; $0090 = 0x01
                    ; $00FF = 0x01

;;;;;;;;;;;;;;;;
; test loading
;;;;;;;;;;;;;;;;

LDA #$00            ; a = 0x00
LDA $10             ; a = $10 = 0x01
LDX $90             ; a = $90 = 0x01
LDY $FF             ; a = $90 = 0x01
NOP                 ; perform assertions:
                    ; a = 0x01
                    ; x = 0x01
                    ; y = 0x01

;;;;;;;;;;;;;;;;
; test zero-page (x-indexed) addressing
;;;;;;;;;;;;;;;;

LDA #$01            ; a = 0x01
LDX #$02            ; x = 0x02
STA $10,X           ; write a=1 to 10+2=$0012
STA $90,X           ; write a=1 to 10+2=$0092
STA $FF,X           ; write a=1 to FF+2=$0001 (should wrap around)
LDX #$90            ; x = 0x90 (testing indices > 0x7F)
STA $11,X           ; write a=1 to 11+90=$00A1
STA $72,X           ; write a=1 to 72+90=$0002
STA $81,X           ; write a=1 to 81+90=$0011
NOP                 ; perform assertions:
                    ; $0012 = 0x01
                    ; $0092 = 0x01
                    ; $0001 = 0x01
                    ; $00A1 = 0x01
                    ; $0001 = 0x01
                    ; $0011 = 0x01

;;;;;;;;;;;;;;;;
; test absolute addressing
;;;;;;;;;;;;;;;;

LDA #$01            ; a = 0x01
STA $0023           ; write a=1 to $0023
STA $0303           ; write a=1 to $0303
STA $0903           ; write a=1 to $0903 ($0103 with mirroring)
STA $0A03           ; write a=1 to $0A03 ($0203 with mirroring)
STA $0F03           ; write a=1 to $0F03 ($0303 with mirroring)
; TODO: test I/O registers
; TODO: test expansion ROM
; TODO: test SRAM
STA $8000           ; write to ROM (should be ignored silently)
STA $FFFF           ; write to ROM (high address)
NOP                 ; perform assertions:
                    ; $0023 = 0x01
                    ; $0303 = 0x01
                    ; $0103 = 0x01
                    ; $0203 = 0x01
                    ; $0303 = 0x01

;;;;;;;;;;;;;;;;
; test absolute (x-indexed) addressing
;;;;;;;;;;;;;;;;

LDX #$02            ; x = 0x02
STA $0023,X         ; write a=1 to 23+2=$0025
STA $0303,X         ; write a=1 to 303+2=$0305
STA $0903,X         ; write a=1 to 903+2=$0905 ($0105 with mirroring)
STA $0A03,X         ; write a=1 to A03+2=$0A03 ($0205 with mirroring)
STA $0F03,X         ; write a=1 to F03+2=$0F03 ($0305 with mirroring)
LDX #$06            ; x = 0x06
STA $FFFF,X         ; write a=1 to FFFF+6=$0005 (wraparound)
STA $7FFC,X         ; write to ROM (should be ignored silently)
STA $FFF0,X         ; write to ROM (high address)
NOP                 ; perform assertions:
                    ; $0025 = 0x01
                    ; $0305 = 0x01
                    ; $0105 = 0x01
                    ; $0205 = 0x01
                    ; $0305 = 0x01
                    ; $0005 = 0x01

;;;;;;;;;;;;;;;;
; test absolute (y-indexed) addressing
;;;;;;;;;;;;;;;;

; these tests are less exhaustive since most of the implementation is shared
; with absolute (x-indexed) mode

LDY #$07            ; y = 0x08
STA $001F,Y         ; write a=1 to 1F+7=$0026
STA $FFFF,Y         ; write a=1 to FFFF+7=$0006 (wraparound)
NOP                 ; perform assertions:
                    ; $0026 = 0x01
                    ; $0006 = 0x01

;;;;;;;;;;;;;;;;
; test indexed indirect addressing
;;;;;;;;;;;;;;;;

LDA #$13            ; a = 0x13
STA $08             ; write a=0x13 to $0008
STA $0A             ; write a=0x13 to $000A
LDA #$02            ; a = 0x02
STA $09             ; write a=0x02 to $0009
STA $03             ; write a=0x02 to $0003
LDA #$03            ; a = 0x03
STA $04             ; write a=0x03 to $0004
LDX #$04            ; x = 0x04
LDA #$01            ; a = 0x01
STA ($04,X)         ; write a=0x01 to 4+4=$0008 -> $0213
STA ($05,X)         ; write a=0x01 to 5+4=$0009 -> $1302
STA ($FF,X)         ; write a=0x01 to FF+4=$0003 -> $0302
NOP                 ; perform assertions:
                    ; 0x0213 = 0x01
                    ; 0x1302 = 0x01
                    ; $0403 = 0x01

;;;;;;;;;;;;;;;;
; test indirect indexed addressing
;;;;;;;;;;;;;;;;

; memory is already set up from previous test

LDY #$02            ; y = 0x02
STA ($08),Y         ; write a=1 to $0008 -> 0213+2=$0215
STA ($09),Y         ; write a=1 to $0009 -> 1302+2=$1304
NOP                 ; perform assertions:
                    ; $0215 = 0x01
                    ; $1304 = 0x01

;;;;;;;;;;;;;;;;
; X register tests
;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;
; preparation
;;;;;;;;;;;;;;;;

LDX #$02            ; x = 0x02
LDY #$04            ; y = 0x04

;;;;;;;;;;;;;;;;
; test zero-page addressing
;;;;;;;;;;;;;;;;

STX $10             ; write x=2 to $0010
STX $90             ; write x=2 to $0090
STX $FF             ; write x=2 to $00FF
NOP                 ; perform assertions:
                    ; $0010 = 0x02
                    ; $0090 = 0x02
                    ; $00FF = 0x02

;;;;;;;;;;;;;;;;
; test zero-page (y-indexed) addressing
;;;;;;;;;;;;;;;;

STX $10,Y           ; write x=2 to 10+4=$0014
STX $90,Y           ; write x=2 to 10+4=$0094
STX $FF,Y           ; write x=2 to FF+4=$0003 (should wrap around)
LDY #$90            ; y = 0x90 (testing indices > 0x7F)
STX $10,Y           ; write x=2 to 10+90=$00A0
STX $70,Y           ; write x=2 to 70+90=$0000
STX $90,Y           ; write x=2 to 90+90=$0020
NOP                 ; perform assertions:
                    ; $0014 = 0x02
                    ; $0094 = 0x02
                    ; $0003 = 0x02
                    ; $00A0 = 0x02
                    ; $0000 = 0x02
                    ; $0020 = 0x02

;;;;;;;;;;;;;;;;
; Y register tests
;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;
; preparation
;;;;;;;;;;;;;;;;

LDY #$04            ; y = 0x04

;;;;;;;;;;;;;;;;
; test zero-page addressing
;;;;;;;;;;;;;;;;

STY $10             ; write x=4 to $0010
STY $90             ; write x=4 to $0090
STY $FF             ; write x=4 to $00FF
NOP                 ; perform assertions:
                    ; $0010 = 0x04
                    ; $0090 = 0x04
                    ; $00FF = 0x04

;;;;;;;;;;;;;;;;
; basic transfer tests
;;;;;;;;;;;;;;;;

LDA #$00            ; reset acc
LDX #$42            ; set x=0x42
LDY #$52            ; set x=0x52

TXA                 ; copy X to acc
STA $08             ; write acc to $0008

TYA                 ; copy Y to acc
STA $09             ; write acc to $0009

NOP                 ; perform assertions:
                    ; $0008 = 0x42
                    ; $0009 = 0x52

; remaining instruction+addressing combos are redundant due to shared implementations

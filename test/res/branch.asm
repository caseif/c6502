;;;;;;;;;;;;;;;;
; test branching
;;;;;;;;;;;;;;;;

; required for execution on the NES
.org $8000

;;;;;;;;;;;;;;;;
; test JMP (indirect)
;;;;;;;;;;;;;;;;

LDX #$00            ; reset X register
LDA #<ind_skip_1    ; low byte of target address
STA $0250
LDA #>ind_skip_1    ; high byte of target address
STA $0251

JMP ($0250)         ; jump to address specified at $0050 ($8012)

LDX #$01            ; set X=1 (this shouldn't execute)

;;; this is offset $8011 in PRG ;;;
ind_skip_1:
NOP                 ; perform assertions:
                    ; x = 0x0

; test page boundary bug
; we stick a label at the bottom offset from the correct label by $1000 bytes
; so that the low byte is the same but the high byte is different

LDX #$00            ; reset X register

LDA #$EE            ; load bogus byte as MSB
STA $0300           ; store after page boundary
LDA #<ind_skip_3    ; fetch lower byte of incorrect label
STA $02FF           ; store before page boundary

LDA #>ind_skip_3    ; fetch upper byte of correct label
STA $0200           ; store after wrong page boundary

JMP ($02FF)

ind_skip_2:
LDX #$01            ; this shouldn't execute

ind_skip_3:
LDX #$02            ; this should execute

ind_end:
NOP                 ; perform assertions:
                    ; x = 0x0

;;;;;;;;;;;;;;;;
; test JMP
;;;;;;;;;;;;;;;;

LDA #$00            ; reset registers
LDX #$00

JMP skip1           ; skip next instruction
LDX #$01            ; set X register (this shouldn't execute)
skip1:  LDA #$01    ; a = 1

NOP                 ; perform assertions:
                    ; a = 0x01
                    ; x = 0x00

;;;;;;;;;;;;;;;;
; test JSR
;;;;;;;;;;;;;;;;

LDX #$05            ; set x
JSR subroutine      ; jump to subroutine
INX                 ; increment X
JMP end_jsr_test

subroutine: INX     ; increment X
            RTS     ; return

LDX #$00            ; reset X (shouldn't execute)

end_jsr_test:
NOP                 ; perform assertions:
                    ; x = 0x07

;;;;;;;;;;;;;;;;
; test BEQ, BNE
;;;;;;;;;;;;;;;;

LDX #$00            ; reset X
LDY #$00            ; reset Y

LDA #$00            ; set a=0 (sets zero flag)

BNE not_equal       ; this should not branch
BEQ equal           ; this should branch

equal:
LDX #$01            ; set x=1
JMP equ_part_2      ; skip the not_equal section

not_equal:          ; this should not be reached
LDX #$02            ; set x=2 (should not execute)

equ_part_2:
LDA #$01            ; set a=1 (clears zero flag)

BEQ equal_2         ; this should not branch
BNE not_equal_2     ; this should branch

equal_2:            ; this shouldn't be reached
LDY #$01            ; set y=1
JMP equ_end         ; skip the not_equal_2 section

not_equal_2:
LDY #$02            ; set y=2

equ_end:
NOP                 ; perform assertions:
                    ; x = 1
                    ; y = 2

;;;;;;;;;;;;;;;;
; test BCC, BCS
;;;;;;;;;;;;;;;;

LDX #$00            ; reset X
LDY #$00            ; reset Y

SEC                 ; set carry flag

BCC no_carry        ; this should not branch
BCS carry           ; this should branch

carry:
LDX #$01            ; set x=1
JMP carry_part_2    ; skip the no_carry section

no_carry:           ; this should not be reached
LDX #$02            ; set x=2

carry_part_2:
CLC                 ; clear carry flag

BCS carry_2         ; this should not branch
BCC no_carry_2      ; this should branch

carry_2:            ; this should not be reached
LDY #$01            ; set y=1
JMP carry_end       ; skip the no_carry_2 section

no_carry_2:
LDY #$02            ; set y=2

carry_end:
NOP                 ; perform assertions:
                    ; x = 1
                    ; y = 2

;;;;;;;;;;;;;;;;
; test BPL, BMI
;;;;;;;;;;;;;;;;

LDX #$00            ; reset X
LDY #$00            ; reset Y

LDA #$01            ; set a to be positive (clears negative flag)

BMI minus           ; this should not branch
BPL plus            ; this should branch

plus:
LDX #$01            ; set x=1
JMP sign_part_2     ; skip the minus section

minus:              ; this should not be reached
LDX #$02            ; set x=2

sign_part_2:
LDA #$80            ; set a to be negative (sets negative flag)

BPL plus_2          ; this should not branch
BMI minus_2         ; this should branch

plus_2:             ; this should not be reached
LDY #$01            ; set y=1
JMP sign_end        ; skip the no_carry_2 section

minus_2:
LDY #$02            ; set y=2

sign_end:
NOP                 ; perform assertions:
                    ; x = 1
                    ; y = 2

; we rely on the test supervisor to set the overflow
; flag here to avoid relying on arithmetic instructions

;;;;;;;;;;;;;;;;
; test BVS, BVC
;;;;;;;;;;;;;;;;

LDX #$00            ; reset X
LDY #$00            ; reset Y

BVC no_overflow     ; this should not branch
BVS overflow        ; this should branch

overflow:
LDX #$01            ; set x=1
JMP overflow_part_2 ; skip the no_carry section

no_overflow:        ; this should not be reached
LDX #$02            ; set x=2

overflow_part_2:
CLV                 ; clear overflow flag

BVS overflow_2      ; this should not branch
BVC no_overflow_2   ; this should branch

overflow_2:         ; this should not be reached
LDY #$01            ; set y=1
JMP overflow_end    ; skip the no_overflow_2 section

no_overflow_2:
LDY #$02            ; set y=2

overflow_end:
NOP                 ; perform assertions:
                    ; x = 1
                    ; y = 2

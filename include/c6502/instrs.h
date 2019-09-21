/*
 * This file is a part of c6502.
 * Copyright (c) 2019, Max Roncace <mproncace@gmail.com>
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    LDA, LDX, LDY, STA, STX, STY, TAX, TAY,
    TSX, TXA, TYA, TXS, ADC, SBC, DEC, DEX,
    DEY, INC, INX, INY, AND, ASL, LSR, BIT,
    EOR, ORA, ROL, ROR, BCC, BCS, BNE, BEQ,
    BPL, BMI, BVC, BVS, JMP, JSR, RTI, RTS,
    CLC, CLD, CLI, CLV, CMP, CPX, CPY, SEC,
    SED, SEI, PHA, PHP, PLA, PLP, BRK, NOP,
    KIL, ANC, SLO, RLA, SRE, RRA, SAX, LAX,
    DCP, ALR, XAA, TAS, SAY, XAS, AXA, ARR,
    LAS, ISC, AXS
} Mnemonic;

typedef enum {
    IMM, ZRP, ZPX, ZPY, ABS, ABX,
    ABY, IND, IZX, IZY, REL, IMP
} AddressingMode;

typedef struct {
    Mnemonic mnemonic;
    AddressingMode addr_mode;
} Instruction;

typedef struct {
    uint8_t value;
    uint16_t raw_operand;
    uint16_t adj_operand;
} InstructionParameter;

typedef enum { INS_OTHER, INS_BRANCH, INS_JUMP, INS_R, INS_W, INS_RW, INS_STACK, INS_REG, INS_RET } InstructionType;

const char *mnemonic_to_str(const Mnemonic mnemonic);

const char *addr_mode_to_str(const AddressingMode addr_mode);

const InstructionType get_instr_type(const Mnemonic mnemonic);

uint8_t get_instr_len(const Instruction *instr);

Instruction *decode_instr(unsigned char opcode);

bool does_cross_page_boundary(uint8_t a, int16_t offset);

bool can_incur_page_boundary_penalty(const uint8_t opcode);

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

#include "c6502/instrs.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static const Instruction g_instr_list[] = {
    {BRK, IMP}, {ORA, IZX}, {KIL, IMP}, {SLO, IZX}, {NOP, ZRP}, {ORA, ZRP}, {ASL, ZRP}, {SLO, ZRP},
    {PHP, IMP}, {ORA, IMM}, {ASL, IMP}, {ANC, IMM}, {NOP, ABS}, {ORA, ABS}, {ASL, ABS}, {SLO, ABS},
    {BPL, REL}, {ORA, IZY}, {KIL, IMP}, {SLO, IZY}, {NOP, ZPX}, {ORA, ZPX}, {ASL, ZPX}, {SLO, ZPX},
    {CLC, IMP}, {ORA, ABY}, {NOP, IMP}, {SLO, ABY}, {NOP, ABX}, {ORA, ABX}, {ASL, ABX}, {SLO, ABX},
    {JSR, ABS}, {AND, IZX}, {KIL, IMP}, {RLA, IZX}, {BIT, ZRP}, {AND, ZRP}, {ROL, ZRP}, {RLA, ZRP},
    {PLP, IMP}, {AND, IMM}, {ROL, IMP}, {ANC, IMM}, {BIT, ABS}, {AND, ABS}, {ROL, ABS}, {RLA, ABS},
    {BMI, REL}, {AND, IZY}, {KIL, IMP}, {RLA, IZY}, {NOP, ZPX}, {AND, ZPX}, {ROL, ZPX}, {RLA, ZPX},
    {SEC, IMP}, {AND, ABY}, {NOP, IMP}, {RLA, ABY}, {NOP, ABX}, {AND, ABX}, {ROL, ABX}, {RLA, ABX},
    {RTI, IMP}, {EOR, IZX}, {KIL, IMP}, {SRE, IZX}, {NOP, ZRP}, {EOR, ZRP}, {LSR, ZRP}, {SRE, ZRP},
    {PHA, IMP}, {EOR, IMM}, {LSR, IMP}, {ALR, IMM}, {JMP, ABS}, {EOR, ABS}, {LSR, ABS}, {SRE, ABS},
    {BVC, REL}, {EOR, IZY}, {KIL, IMP}, {SRE, IZY}, {NOP, ZPX}, {EOR, ZPX}, {LSR, ZPX}, {SRE, ZPX},
    {CLI, IMP}, {EOR, ABY}, {NOP, IMP}, {SRE, ABY}, {NOP, ABX}, {EOR, ABX}, {LSR, ABX}, {SRE, ABX},
    {RTS, IMP}, {ADC, IZX}, {KIL, IMP}, {RRA, IZX}, {NOP, ZRP}, {ADC, ZRP}, {ROR, ZRP}, {RRA, ZRP},
    {PLA, IMP}, {ADC, IMM}, {ROR, IMP}, {ARR, IMM}, {JMP, IND}, {ADC, ABS}, {ROR, ABS}, {RRA, ABS},
    {BVS, REL}, {ADC, IZY}, {KIL, IMP}, {RRA, IZY}, {NOP, ZPX}, {ADC, ZPX}, {ROR, ZPX}, {RRA, ZPX},
    {SEI, IMP}, {ADC, ABY}, {NOP, IMP}, {RRA, ABY}, {NOP, ABX}, {ADC, ABX}, {ROR, ABX}, {RRA, ABX},
    {NOP, IMM}, {STA, IZX}, {NOP, IMM}, {SAX, IZX}, {STY, ZRP}, {STA, ZRP}, {STX, ZRP}, {SAX, ZRP},
    {DEY, IMP}, {NOP, IMM}, {TXA, IMP}, {XAA, IMM}, {STY, ABS}, {STA, ABS}, {STX, ABS}, {SAX, ABS},
    {BCC, REL}, {STA, IZY}, {KIL, IMP}, {AXA, IZY}, {STY, ZPX}, {STA, ZPX}, {STX, ZPY}, {SAX, ZPY},
    {TYA, IMP}, {STA, ABY}, {TXS, IMP}, {TAS, ABY}, {SAY, ABX}, {STA, ABX}, {XAS, ABY}, {AXA, ABY},
    {LDY, IMM}, {LDA, IZX}, {LDX, IMM}, {LAX, IZX}, {LDY, ZRP}, {LDA, ZRP}, {LDX, ZRP}, {LAX, ZRP},
    {TAY, IMP}, {LDA, IMM}, {TAX, IMP}, {LAX, IMM}, {LDY, ABS}, {LDA, ABS}, {LDX, ABS}, {LAX, ABS},
    {BCS, REL}, {LDA, IZY}, {KIL, IMP}, {LAX, IZY}, {LDY, ZPX}, {LDA, ZPX}, {LDX, ZPY}, {LAX, ZPY},
    {CLV, IMP}, {LDA, ABY}, {TSX, IMP}, {LAS, ABY}, {LDY, ABX}, {LDA, ABX}, {LDX, ABY}, {LAX, ABY},
    {CPY, IMM}, {CMP, IZX}, {NOP, IMM}, {DCP, IZX}, {CPY, ZRP}, {CMP, ZRP}, {DEC, ZRP}, {DCP, ZRP},
    {INY, IMP}, {CMP, IMM}, {DEX, IMP}, {AXS, IMM}, {CPY, ABS}, {CMP, ABS}, {DEC, ABS}, {DCP, ABS},
    {BNE, REL}, {CMP, IZY}, {KIL, IMP}, {DCP, IZY}, {NOP, ZPX}, {CMP, ZPX}, {DEC, ZPX}, {DCP, ZPX},
    {CLD, IMP}, {CMP, ABY}, {NOP, IMP}, {DCP, ABY}, {NOP, ABX}, {CMP, ABX}, {DEC, ABX}, {DCP, ABX},
    {CPX, IMM}, {SBC, IZX}, {NOP, IMM}, {ISC, IZX}, {CPX, ZRP}, {SBC, ZRP}, {INC, ZRP}, {ISC, ZRP},
    {INX, IMP}, {SBC, IMM}, {NOP, IMP}, {SBC, IMM}, {CPX, ABS}, {SBC, ABS}, {INC, ABS}, {ISC, ABS},
    {BEQ, REL}, {SBC, IZY}, {KIL, IMP}, {ISC, IZY}, {NOP, ZPX}, {SBC, ZPX}, {INC, ZPX}, {ISC, ZPX},
    {SED, IMP}, {SBC, ABY}, {NOP, IMP}, {ISC, ABY}, {NOP, ABX} ,{SBC, ABX}, {INC, ABX}, {ISC, ABX} 
};

const char *g_mnemonic_strs[] = {
    "LDA", "LDX", "LDY", "STA", "STX", "STY", "TAX", "TAY",
    "TSX", "TXA", "TYA", "TXS", "ADC", "SBC", "DEC", "DEX",
    "DEY", "INC", "INX", "INY", "AND", "ASL", "LSR", "BIT",
    "EOR", "ORA", "ROL", "ROR", "BCC", "BCS", "BNE", "BEQ",
    "BPL", "BMI", "BVC", "BVS", "JMP", "JSR", "RTI", "RTS",
    "CLC", "CLD", "CLI", "CLV", "CMP", "CPX", "CPY", "SEC",
    "SED", "SEI", "PHA", "PHP", "PLA", "PLP", "BRK", "NOP",
    "KIL", "ANC", "SLO", "RLA", "SRE", "RRA", "SAX", "LAX",
    "DCP", "ALR", "XAA", "TAS", "SAY", "XAS", "AXA", "ARR",
    "LAS", "ISC", "AXS"
};

const char *g_addr_mode_strs[] = {
    "IMM", "ZRP", "ZPX", "ZPY", "ABS", "ABX",
    "ABY", "IND", "IZX", "IZY", "REL", "IMP"
};

const char *mnemonic_to_str(const Mnemonic mnemonic) {
    return g_mnemonic_strs[(unsigned int) mnemonic];
}

const char *addr_mode_to_str(const AddressingMode addr_mode) {
    return g_addr_mode_strs[(unsigned int) addr_mode];
}

const InstructionType get_instr_type(const Mnemonic mnemonic) {
    switch (mnemonic) {
        case LDA:
        case LDX:
        case LDY:
        case ADC:
        case SBC:
        case AND:
        case BIT:
        case EOR:
        case ORA:
        case CMP:
        case CPX:
        case CPY:
        // unofficial
        case NOP:
        case LAX:
        case ANC:
        case ALR:
        case XAA:
        case ARR: // matey
        case LAS:
            return INS_R;
        case STA:
        case STX:
        case STY:
        // unofficial
        case SAX:
        case AXS:
            return INS_W;
        case DEC:
        case INC:
        case ASL:
        case LSR:
        case ROL:
        case ROR:
        // unofficial
        case SLO:
        case DCP:
        case ISC:
        case RLA:
        case SRE:
        case RRA:
        case SAY:
        case XAS:
        case AXA:
            return INS_RW;
        case BCC:
        case BCS:
        case BNE:
        case BEQ:
        case BPL:
        case BMI:
        case BVC:
        case BVS:
            return INS_BRANCH;
        case JMP:
        case JSR:
            return INS_JUMP;
        case PHA:
        case PLA:
        case PHP:
        case PLP:
            return INS_STACK;
        case TAX:
        case TAY:
        case TXA:
        case TYA:
        case INX:
        case INY:
        case DEX:
        case DEY:
        case TSX:
        case TXS:
        case CLC:
        case SEC:
        case CLI:
        case SEI:
        case CLV:
        case CLD:
        case SED:
        // unofficial
        case TAS:
            return INS_REG;
        case RTS:
        case RTI:
            return INS_RET;
        case BRK:
        case KIL:
            return INS_OTHER;
        default:
            assert(false);
    }
}

uint8_t get_instr_len(const Instruction *instr) {
    // BRK has a padding byte
    if (instr->mnemonic == BRK) {
        return 2;
    }

    switch (instr->addr_mode) {
        case IMP:
            return 1;
        case IMM:
        case ZRP:
        case ZPX:
        case ZPY:
        case IZX:
        case IZY:
        case REL:
            return 2;
        case ABS:
        case ABX:
        case ABY:
        case IND:
            return 3;
        default:
            printf("get_instr_len: Unhandled case %d", instr->addr_mode);
            assert(0);
    }
}

Instruction *decode_instr(unsigned char opcode) {
    return (Instruction*) &g_instr_list[opcode];
}

bool can_incur_page_boundary_penalty(const uint8_t opcode) {
    // all opcodes with an even high nybble don't incur penalties
    if (!((opcode >> 4) & 1)) {
        return false;
    }

    uint8_t low_nybble = opcode & 0xF;

    if ((opcode >> 4) == 0x9) {
        // for all opcodes 0x9X, only 0x90 incurs a penalty
        return low_nybble == 0;
    } else if ((opcode >> 4) == 0xB) {
        // row 0xBX is unusual - I can't find a pattern in it
        if (low_nybble == 0x3 || low_nybble == 0xB || low_nybble == 0xE || low_nybble == 0xF) {
            return true;
        }
    }

    return low_nybble == 0x0 || low_nybble == 0x1 || low_nybble == 0x9 || low_nybble == 0xC || low_nybble == 0xD;
}

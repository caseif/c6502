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

#include "c6502/instrs.h"

#include <stdbool.h>
#include <stdint.h>

#define PACKED __attribute__((packed))

typedef union {
    struct {
        unsigned char carry:1 PACKED;
        unsigned char zero:1 PACKED;
        unsigned char interrupt_disable:1 PACKED;
        unsigned char decimal:1 PACKED;
        unsigned char break_command:1 PACKED;
        unsigned char unused:1 PACKED;
        unsigned char overflow:1 PACKED;
        unsigned char negative:1 PACKED;
    };
    unsigned char serial:8;
} StatusRegister;

typedef struct {
    StatusRegister status;
    uint16_t pc;
    uint8_t sp;
    uint8_t acc;
    uint8_t x;
    uint8_t y;
} CpuRegisters;

typedef struct {
    uint16_t vector_loc;
    bool maskable;
    bool push_pc;
    bool set_b;
    bool set_i;
} InterruptType;

typedef struct {
    uint8_t (*mem_read)(uint16_t);
    void (*mem_write)(uint16_t, uint8_t);
    uint8_t (*bus_read)(void);
    void (*bus_write)(uint8_t);
} CpuSystemInterface;

extern const InterruptType INT_RESET;
extern const InterruptType INT_NMI;
extern const InterruptType INT_IRQ;
extern const InterruptType INT_BRK;

void initialize_cpu(CpuSystemInterface system_iface);

CpuRegisters *cpu_get_registers(void);

uint8_t cpu_get_instruction_step(void);

Instruction *cpu_get_current_instruction(void);

void cpu_set_log_callback(void (*callback)(char*, CpuRegisters));

void cpu_raise_nmi_line(void);

void cpu_clear_nmi_line(void);

void cpu_raise_irq_line(void);

void cpu_clear_irq_line(void);

void cpu_raise_rst_line(void);

void cpu_clear_rst_line(void);

bool issue_interrupt(const InterruptType *type);

void cycle_cpu(void);

char *cpu_print_current_instruction(char *target);

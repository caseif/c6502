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

#include "cpu/cpu_tester.h"

#include "cpu/cpu.h"
#include "cpu/instrs.h"
#include "system.h"
#include "test_assert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern CpuRegisters g_cpu_regs;

extern bool test_addition(void);
extern bool test_arithmetic(void);
extern bool test_branch(void);
extern bool test_interrupt(void);
extern bool test_logic(void);
extern bool test_stack(void);
extern bool test_status(void);
extern bool test_store_load(void);
extern bool test_subtraction(void);

static DataBlob _load_file(FILE *file) {
    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *data = malloc(size);
    if (!data || !fread(data, size, 1, file)) {
        printf("Failed to read file\n");
        exit(-1);
    }

    return (DataBlob) {data, size};
}

void load_cpu_test(char *file_name) {
    char *prefix = "test/res/";
    char *qualified = malloc(strlen(file_name) + strlen(prefix) + 1);
    sprintf(qualified, "%s%s", prefix, file_name);

    FILE *program_file = fopen(qualified, "rb");

    if (!program_file) {
        printf("Could not open program file %s.\n", file_name);
    }

    DataBlob program = _load_file(program_file);

    if (!program.data) {
        printf("Failed to load program %s.\n", file_name);
        exit(-1);
    }

    Cartridge *cart = (Cartridge*) malloc(sizeof(Cartridge));
    cart->prg_rom = (unsigned char*) malloc(program.size);
    memcpy(cart->prg_rom, program.data, program.size);
    cart->prg_size = program.size;
    cart->chr_size = 0;
    cart->mapper = (Mapper*) malloc(sizeof(Mapper));
    mapper_init_nrom(cart->mapper);
    cart->title = qualified;

    initialize_system(cart);

    printf("Successfully loaded program file %s.\n", file_name);
}

extern Instruction *g_cur_instr;
extern uint8_t g_instr_cycle;

void pump_cpu(void) {
    do {
        cycle_cpu();
    } while (!(g_cur_instr->mnemonic == NOP && g_instr_cycle == 1));
}

bool do_cpu_tests(void) {
    bool res = true;

    res &= test_addition();
    res &= test_arithmetic();
    res &= test_branch();
    res &= test_interrupt();
    res &= test_logic();
    res &= test_stack();
    res &= test_status();
    res &= test_store_load();
    res &= test_subtraction();

    return res;
}

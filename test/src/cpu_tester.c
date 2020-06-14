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

#include "cpu_tester.h"
#include "test_assert.h"

#include "c6502/cpu.h"
#include "c6502/instrs.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern bool test_addition(void);
extern bool test_arithmetic(void);
extern bool test_branch(void);
extern bool test_interrupt(void);
extern bool test_logic(void);
extern bool test_stack(void);
extern bool test_status(void);
extern bool test_store_load(void);
extern bool test_subtraction(void);

static unsigned char g_sys_ram[0x800];
static uint8_t g_sys_bus;
static DataBlob g_program;

static char *g_res_prefix;

static DataBlob _load_file(FILE *file) {
    if (fseek(file, 0L, SEEK_END) != 0) {
        printf("Failed to seek to file end\n");
        exit(-1);
    }
    size_t size = ftell(file);
    if (fseek(file, 0, SEEK_SET) != 0) {
        printf("Failed to seek to file start\n");
        exit(-1);
    }

    unsigned char *data = malloc(size);
    if (!data || !fread(data, size, 1, file)) {
        printf("Failed to read file (errno: %d)\n", errno);
        exit(-1);
    }

    return (DataBlob) {data, size};
}

static void _log_callback(char *instr_str, CpuRegisters last_regs) {
    printf("%04X %s %02X\n", last_regs.pc, instr_str, last_regs.sp);
}

uint8_t system_memory_read(uint16_t addr) {
    if (addr < 0x2000) {
        return g_sys_ram[addr % sizeof(g_sys_ram)];
    } else if (addr >= 0x8000) {
        addr %= 0x8000;

         // mirroring
        if (addr >= 0x4000 && g_program.size <= 0x4000) {
            addr -= 0x4000;
        }

        if (addr >= g_program.size) {
            return 0;
        }

        return g_program.data[addr];
    } else {
        return 0;
    }
}

void system_memory_write(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) {
        g_sys_ram[addr % sizeof(g_sys_ram)] = val;
    } else if (addr >= 0x8000) {
        addr %= 0x8000;

        // mirroring
        if (addr >= 0x4000 && g_program.size <= 0x4000) {
            addr -= 0x4000;
        }

        if (addr >= g_program.size) {
            return;
        }

        g_program.data[addr] = val;
    }
}

uint8_t system_bus_read(void) {
    return g_sys_bus;
}

void system_bus_write(uint8_t val) {
    g_sys_bus = val;
}

unsigned int poll_nmi_line(void) {
    return 1;
}

unsigned int poll_irq_line(void) {
    return 1;
}

unsigned int poll_rst_line(void) {
    return 1;
}


bool load_cpu_test(char *file_name) {
    char *qualified = malloc(strlen(file_name) + strlen(g_res_prefix) + 2);
    sprintf(qualified, "%s/%s", g_res_prefix, file_name);

    FILE *program_file = fopen(qualified, "rb");

    if (!program_file) {
        printf("Could not open program file %s. Errno: %d\n", file_name, errno);
        return false;
    }

    g_program = _load_file(program_file);

    if (!g_program.data) {
        printf("Failed to load program %s.\n", file_name);
        exit(-1);
    }

    memset(g_sys_ram, 0, sizeof(g_sys_ram));

    initialize_cpu((CpuSystemInterface){
            system_memory_read,
            system_memory_write,
            system_bus_read,
            system_bus_write,
            poll_nmi_line,
            poll_irq_line,
            poll_rst_line
    });
    //cpu_set_log_callback(_log_callback);

    printf("Successfully loaded program file %s.\n", file_name);
}

void unload_cpu_test() {
    free(g_program.data);
}

void pump_cpu(void) {
    do {
        cycle_cpu();
    } while (!(cpu_get_current_instruction() != NULL
            && cpu_get_current_instruction()->mnemonic == NOP
            && cpu_get_instruction_step() == 1));
}

bool do_cpu_tests(char *res_prefix) {
    g_res_prefix = res_prefix;

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

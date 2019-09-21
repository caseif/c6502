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

#include "test_assert.h"
#include "cpu/cpu_tester.h"

#include "cpu/cpu.h"

extern CpuRegisters g_cpu_regs;

bool test_status(void) {
    load_cpu_test("status.bin");

    // test explicit flag-setting
    pump_cpu();
    ASSERT_EQ(1, g_cpu_regs.status.carry);
    ASSERT_EQ(1, g_cpu_regs.status.interrupt_disable);

    // set overflow flag explicitly so we can test clearing it in code
    g_cpu_regs.status.overflow = 1;

    // test explicit flag-clearing
    pump_cpu();
    ASSERT_EQ(0, g_cpu_regs.status.carry);
    ASSERT_EQ(0, g_cpu_regs.status.interrupt_disable);
    ASSERT_EQ(0, g_cpu_regs.status.overflow);

    // test comparison instructions

    // test CMP

    // a = v
    pump_cpu();
    ASSERT_EQ(1, g_cpu_regs.status.carry);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(1, g_cpu_regs.status.zero);

    // a > v
    pump_cpu();
    ASSERT_EQ(1, g_cpu_regs.status.carry);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    // a < v
    pump_cpu();
    ASSERT_EQ(0, g_cpu_regs.status.carry);
    ASSERT_EQ(1, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    // a > v && a is neg && v is neg
    pump_cpu();
    ASSERT_EQ(1, g_cpu_regs.status.carry);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    // a > v && a is neg && v is pos
    pump_cpu();
    ASSERT_EQ(1, g_cpu_regs.status.carry);
    ASSERT_EQ(1, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    // a < v && a is neg && v is neg
    pump_cpu();
    ASSERT_EQ(0, g_cpu_regs.status.carry);
    ASSERT_EQ(1, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    // test CPX

    // x == v
    pump_cpu();
    ASSERT_EQ(1, g_cpu_regs.status.carry);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(1, g_cpu_regs.status.zero);

    // test CPY

    // y == v
    pump_cpu();
    ASSERT_EQ(1, g_cpu_regs.status.carry);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(1, g_cpu_regs.status.zero);

    return true;
}

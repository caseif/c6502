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

bool test_arithmetic(void) {
    load_cpu_test("arithmetic.bin");

    // test INX

    pump_cpu();
    ASSERT_EQ(1, g_cpu_regs.x);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0, g_cpu_regs.x);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(1, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0x80, g_cpu_regs.x);
    ASSERT_EQ(1, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0x81, g_cpu_regs.x);
    ASSERT_EQ(1, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    // test INY

    pump_cpu();
    ASSERT_EQ(1, g_cpu_regs.y);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0, g_cpu_regs.y);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(1, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0x80, g_cpu_regs.y);
    ASSERT_EQ(1, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0x81, g_cpu_regs.y);
    ASSERT_EQ(1, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    // test INC

    pump_cpu();
    ASSERT_EQ(1, g_cpu_regs.acc);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0, g_cpu_regs.acc);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(1, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0x80, g_cpu_regs.acc);
    ASSERT_EQ(1, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0x81, g_cpu_regs.acc);
    ASSERT_EQ(1, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    // test DEX

    pump_cpu();
    ASSERT_EQ(1, g_cpu_regs.x);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0, g_cpu_regs.x);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(1, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0xFF, g_cpu_regs.x);
    ASSERT_EQ(1, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0x7F, g_cpu_regs.x);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    // test DEY

    pump_cpu();
    ASSERT_EQ(1, g_cpu_regs.y);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0, g_cpu_regs.y);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(1, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0xFF, g_cpu_regs.y);
    ASSERT_EQ(1, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0x7F, g_cpu_regs.y);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    // test DEC

    pump_cpu();
    ASSERT_EQ(1, g_cpu_regs.acc);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0, g_cpu_regs.acc);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(1, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0xFF, g_cpu_regs.acc);
    ASSERT_EQ(1, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    pump_cpu();
    ASSERT_EQ(0x7F, g_cpu_regs.acc);
    ASSERT_EQ(0, g_cpu_regs.status.negative);
    ASSERT_EQ(0, g_cpu_regs.status.zero);

    return true;
}

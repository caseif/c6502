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

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc > 2) {
        printf("Too many params!\nUsage: c6502_test [resource prefix]");
        return 1;
    }

    char *res_prefix = ".";

    if (argc == 2) {
        res_prefix = argv[1];
        printf("Using prefix %s\n", res_prefix);
    }

    printf("Starting CPU tests...\n");

    if (do_cpu_tests(res_prefix)) {
        printf("All CPU tests completed successfully.\n");
    } else {
        printf("CPU tests failed!\n");
        return -1;
    }

    return 0;
}

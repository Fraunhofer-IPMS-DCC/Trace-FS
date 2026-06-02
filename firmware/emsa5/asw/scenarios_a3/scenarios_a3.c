/*
 * AbsInt Example Program
 * (C) AbsInt Angewandte Informatik GmbH
 *
 * Small Scenario Example
 */

#include "defs.h"
#include "dhrystone.h"
#include "minmax.h"
#include "trace_contexts.h"

/* ================================================== *
 *          5ms TASK                                 *
 * ================================================== */

/*  uses trace_contexts.c routines bs/math */

volatile int signal;
volatile unsigned char bs_math_index;

void task_5ms_bs_math(void) {

    if (signal) {
        bs_math_index = 10;
    } else {
        bs_math_index = 20;
    };

    if (bs_math_index==10) {
        bs();
    }
    if (bs_math_index==20) {
        math();
    }
    return;
}


/* ================================================== *
 *          10ms TASK - Dhrystone Benchmark           *
 * ================================================== */

void task_10ms_dhry(void) {
    /*  call the lovely Dhrystone Benchmark */
    dhry();
    return;
}


/* ================================================== *
 *          ISR2 is MIN-MAX                           *
 * ================================================== */

int result = 0;
volatile int x;
volatile int y;
volatile int z;

void ISR2_minmax (void) {
    if (x <= y) {
        swap (&x, &y);
    }
    else  if (x <= z)  {
        x += min (x, y, z);
    }
    else {
        z *= max (z, y, x);
    }
    result = (y <= z ? y+z : y-z);
    return;
}

/* ================================================== *
 *          100ms TASK - many branches                *
 * ================================================== */

const int init_vector [20] = { 90, 47, 85, 66, 49, 7, 93, 35, 78, 63, 90, 47, 85, 66, 49, 7, 93, 35, 78, 63 };

int f (int x)
{
    volatile int return_value = 255;

    if (x & (0x1 << 0))
        return_value += x;
    if (x & (0x1 << 1))
        return_value *= x;
    if (x & (0x1 << 2))
        return_value -= x;
    if (x & (0x1 << 3))
        return_value /= x;
    if (x & (0x1 << 4))
        return_value >>= x;
    if (x & (0x1 << 5))
        return_value &= x;
    if (x & (0x1 << 6))
        return_value ^= x;
    if (x & (0x1 << 7))
        return_value |= x;

    return return_value;
}

void task_100ms_nexus (void)
{
    signal = z % 3 == 0;
    y = f(init_vector[x%20]);
    z++;
}

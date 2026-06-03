/**
  * Firmware for TRISTAN TRACE_FS demonstrator.
  *
  * (c) 2025-2026 AbsInt Angewandte Informatik GmbH
  *
  * Author:  Simon Wegener
  * License: MIT
  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "irq.h"
#include "ctrace.h"
#include "scheduler.h"
#include "timer.h"

#define NUM_TASKS 4
#define SLOWDOWN 25
#define ONE_MILLISECOND 1000

void task_5ms (void);
void task_10ms (void);
void task_100ms (void);
void task_1000ms (void);

const task_t tasks[NUM_TASKS] = {
    { 1, task_5ms, 5, 0 },
    { 2, task_10ms, 10, 1 },
    { 3, task_100ms, 100, 2 },
    { 4, task_1000ms, 1000, 3 },
};

const task_t * volatile scheduler_queue[NUM_TASKS];
volatile uint8_t scheduler_enabled;
volatile uint32_t scheduler_ticks;

void scheduler_init (void)
{
    scheduler_ticks = 0;
    for (int i = 0; i < NUM_TASKS; i++)
        scheduler_queue[i] = NULL;

    // one tick each ms
    timer_configure_periodic_timer(TIMER_0, SLOWDOWN * ONE_MILLISECOND);

    scheduler_enable();
}

void scheduler_enable (void)
{
    scheduler_enabled = 1;
}

void scheduler_disable (void)
{
    scheduler_enabled = 0;
    timer_disable_timer(TIMER_0);
}

void scheduler_run (void)
{
    while (scheduler_enabled) {
#if 1
        for (int i = 0; i < NUM_TASKS; i++) {
            if (scheduler_queue[i]) {
                __builtin_ctrace_ownership_trace(scheduler_queue[i]->id);
                scheduler_queue[i]->entry();
                scheduler_queue[i] = NULL;
            }
        }
#else
        if (scheduler_queue[0]) {
            __builtin_ctrace_ownership_trace(scheduler_queue[0]->id);
            task_5ms();
            scheduler_queue[0] = NULL;
        }
        if (scheduler_queue[1]) {
            __builtin_ctrace_ownership_trace(scheduler_queue[1]->id);
            task_10ms();
            scheduler_queue[1] = NULL;
        }
        if (scheduler_queue[2]) {
            __builtin_ctrace_ownership_trace(scheduler_queue[2]->id);
            task_100ms();
            scheduler_queue[2] = NULL;
        }
        if (scheduler_queue[3]) {
            __builtin_ctrace_ownership_trace(scheduler_queue[3]->id);
            task_1000ms();
            scheduler_queue[3] = NULL;
        }
#endif
    }
}

void scheduler_next (void)
{
    scheduler_ticks++;
    uint32_t time_in_ms = scheduler_ticks;

    for (int i = 0; i < NUM_TASKS; i++) {
        if (time_in_ms % tasks[i].period == 0)
            scheduler_queue[i] = &tasks[i];
    }
}



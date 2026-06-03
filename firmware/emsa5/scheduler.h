/**
  * Firmware for TRISTAN TRACE_FS demonstrator.
  *
  * (c) 2025-2026 AbsInt Angewandte Informatik GmbH
  *
  * Author:  Simon Wegener
  * License: MIT
  */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef void (*entry_func_t) (void);

typedef struct task_t {
    uint16_t id;
    entry_func_t entry;
    uint32_t period;
    uint32_t priority;
} task_t;

void scheduler_init (void);
void scheduler_enable (void);
void scheduler_disable (void);
void scheduler_run (void);
void scheduler_next (void);

#endif /* SCHEDULER_H */

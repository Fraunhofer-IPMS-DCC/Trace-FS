/**
  * Hardware Abstraction Layer for TRISTAN TRACE_FS demonstrator.
  *
  * (c) 2025-2026 AbsInt Angewandte Informatik GmbH
  *
  * Author:  Simon Wegener
  * License: MIT
  */

#ifndef CORE_H
#define CORE_H

#include "csr.h"

void core_disable_interrupts(void);
void core_disable_external_interrupt(void);
void core_disable_timer_interrupt(void);

void core_enable_interrupts(void);
void core_enable_external_interrupt(void);
void core_enable_timer_interrupt(void);

static inline void core_busy_waiting (uint32_t cycles)
{
    while (cycles--)
    {
        // busy waiting
    }
}

#endif /* CORE_H */

/**
  * Hardware Abstraction Layer for TRISTAN TRACE_FS demonstrator.
  *
  * (c) 2025-2026 AbsInt Angewandte Informatik GmbH
  *
  * Author:  Simon Wegener
  * License: MIT
  */

#ifndef TIMER_H
#define TIMER_H

typedef enum {
    TIMER_0 = 0,
    TIMER_1 = 1,
} timer_t;

void timer_init (void);

void timer_acknowledge_timer (timer_t timer);
void timer_configure_periodic_timer (timer_t timer, uint32_t period_in_us);
void timer_disable_timer (timer_t timer);

#endif /* TIMER_H */

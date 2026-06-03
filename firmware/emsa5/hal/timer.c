/**
  * Hardware Abstraction Layer for TRISTAN TRACE_FS demonstrator.
  *
  * (c) 2025-2026 AbsInt Angewandte Informatik GmbH
  *
  * Author:  Simon Wegener
  * License: MIT
  */

#include "axi_timer.h"
#include "timer.h"

volatile axi_timer_t axi_timer __attribute((section(".axi_timer")));

#define AXI_TIMER_CLOCK 100000000

void timer_init (void)
{
    // reset timer 0 state
    axi_timer.tcsr0 = 0x00000100;

    // reset timer 1 state
    axi_timer.tcsr1 = 0x00000100;
}

void timer_acknowledge_timer (timer_t timer)
{
    if (timer == TIMER_0) {
        axi_timer_tcsr0_t tcsr0 = { .full_register = axi_timer.tcsr0 };

        tcsr0.t0int = 1;

        axi_timer.tcsr0 = tcsr0.full_register;
    } else if (timer == TIMER_1) {
        axi_timer.tcsr1 = axi_timer.tcsr1 | 0x00000100;
    } else {
//        fatal_error();
    }
}

void timer_configure_periodic_timer (timer_t timer, uint32_t period_in_us)
{
    if (timer == TIMER_0) {
        // reset timer 0 state
        axi_timer.tcsr0 = 0x00000100;

        // set timer 0 interval (auto-reload value)
        axi_timer.tlr0 = 0xffffffff - (period_in_us * (AXI_TIMER_CLOCK / 1000000));

        // preload timer 0 value
        axi_timer.tcsr0 = 0x00000020;

        // now start periodic timer
        axi_timer_tcsr0_t tcsr0 = { .full_register = axi_timer.tcsr0 };

        tcsr0.ent0 = 1;  // enable timer 0
        tcsr0.enit0 = 1; // enable interrupt for timer 0
        tcsr0.load0 = 0; // do not load auto reload value into timer 0 (would prevent timer from running)
        tcsr0.arht0 = 1; // enable auto-reload mode
        tcsr0.capt0 = 0; // disable external capture trigger timer 0
        tcsr0.gent0 = 0; // disable external generate signal timer 0
        tcsr0.udt0 = 0;  // count upwards
        tcsr0.mdt0 = 0;  // set timer mode to generate

        axi_timer.tcsr0 = tcsr0.full_register;
    } else if (timer == TIMER_1) {
        // FIXME
    } else {
//        fatal_error();
    }
}

void timer_disable_timer (timer_t timer)
{
    if (timer == TIMER_0) {
        axi_timer_tcsr0_t tcsr0 = { .full_register = axi_timer.tcsr0 };

        tcsr0.ent0 = 0;  // disable timer 0
        tcsr0.enit0 = 0; // disable interrupt for timer 0

        axi_timer.tcsr0 = tcsr0.full_register;
    } else if (timer == TIMER_1) {
        // FIXME
    } else {
//        fatal_error();
    }
}


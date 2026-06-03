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

#include "core.h"
#include "gpio.h"
#include "heartbeat.h"
#include "irq.h"
#include "led.h"
#include "scheduler.h"
#include "timer.h"
#include "asw/scenarios_a3/scenarios_a3.h"

void handle_timer_interrupt (void)
{
    heartbeat_beat();
    scheduler_next();
}

void handle_gpio_interrupt (void)
{
    ISR2_minmax();
}

uint32_t handle_trap (uint32_t mcause, uint32_t mepc)
{
    core_disable_interrupts();

    if (mcause & 0x80000000) {

        // timer interrupt
        if (irq_is_pending_interrupt(IRQ_0)) {
            timer_acknowledge_timer(TIMER_0);
            timer_acknowledge_timer(TIMER_1);

            irq_acknowledge_interrupt(IRQ_0);
            led_toggle(LED_0, ON);

            handle_timer_interrupt();
        } else {
            led_toggle(LED_0, OFF);
        }

        if (irq_is_pending_interrupt(IRQ_1)) {
            irq_acknowledge_interrupt(IRQ_1);
            led_toggle(LED_1, ON);
        } else {
            led_toggle(LED_1, OFF);
        }

        if (irq_is_pending_interrupt(IRQ_2)) {
            gpio_acknowledge_interrupt();

            irq_acknowledge_interrupt(IRQ_2);
            led_toggle(LED_2, ON);

            handle_gpio_interrupt();
        } else {
            led_toggle(LED_2, OFF);
        }

        if (irq_is_pending_interrupt(IRQ_3)) {
            irq_acknowledge_interrupt(IRQ_3);
            led_toggle(LED_3, ON);
        } else {
            led_toggle(LED_3, OFF);
        }
    }

    core_enable_interrupts();

    return mepc;
}


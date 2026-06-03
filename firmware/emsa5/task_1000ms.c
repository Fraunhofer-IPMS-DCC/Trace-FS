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
#include "led.h"
#include "scheduler.h"
#include "asw/asm_test/asm_test.h"

#define WAIT_TIME 0x00080000

void task_1000ms (void)
{
    scheduler_disable();

    led_toggle(LED_BLOCK_1, ON);
    core_busy_waiting(WAIT_TIME);
    asm_test_main();
    led_toggle(LED_BLOCK_1, OFF);
}

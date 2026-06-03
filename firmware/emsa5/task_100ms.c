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

#include "scheduler.h"
#include "asw/ctrace_test/ctrace_test.h"
#include "asw/scenarios_a3/scenarios_a3.h"

void task_100ms (void)
{
    task_100ms_nexus();

    ctrace_test_main();
}

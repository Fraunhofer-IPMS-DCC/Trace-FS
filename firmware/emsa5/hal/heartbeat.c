/**
  * Hardware Abstraction Layer for TRISTAN TRACE_FS demonstrator.
  *
  * (c) 2025-2026 AbsInt Angewandte Informatik GmbH
  *
  * Author:  Simon Wegener
  * License: MIT
  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "heartbeat.h"

volatile uint32_t heartbeat_counter __attribute((section(".heartbeat")));

void heartbeat_init (void)
{
    heartbeat_counter = 0;
}

void heartbeat_beat (void)
{
    heartbeat_counter++;
}


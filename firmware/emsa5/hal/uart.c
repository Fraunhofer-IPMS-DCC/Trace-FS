/**
  * Hardware Abstraction Layer for TRISTAN TRACE_FS demonstrator.
  *
  * (c) 2025-2026 AbsInt Angewandte Informatik GmbH
  *
  * Author:  Simon Wegener
  * License: MIT
  */

#include "axi_uartlite.h"

volatile axi_uartlite_t axi_uartlite __attribute((section(".axi_uartlite")));

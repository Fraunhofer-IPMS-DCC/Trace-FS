/**
  * Hardware Abstraction Layer for TRISTAN TRACE_FS demonstrator.
  *
  * (c) 2025-2026 AbsInt Angewandte Informatik GmbH
  *
  * Basic definitions for Xilinx AXI UART Lite (PG142).
  * See https://docs.amd.com/v/u/en-US/pg142-axi-uartlite for more details.
  *
  * Author:  Simon Wegener
  * License: MIT
  */

#ifndef AXI_UARTLITE_H
#define AXI_UARTLITE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern volatile uint32_t __axi_uartlite_base;

typedef volatile struct {
    uint32_t rx_fifo;
    uint32_t tx_fifo;
    uint32_t stat_reg;
    uint32_t ctrl_reg;
} axi_uartlite_t;

#endif /* AXI_UARTLITE_H */

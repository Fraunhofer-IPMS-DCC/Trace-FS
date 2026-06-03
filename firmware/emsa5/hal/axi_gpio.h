/**
  * Hardware Abstraction Layer for TRISTAN TRACE_FS demonstrator.
  *
  * (c) 2025-2026 AbsInt Angewandte Informatik GmbH
  *
  * Basic definitions for Xilinx AXI GPIO (PG144).
  * See https://docs.amd.com/r/en-US/pg144-axi-gpio/ for more details.
  *
  * Author:  Simon Wegener
  * License: MIT
  */

#ifndef AXI_GPIO_H
#define AXI_GPIO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern volatile uint32_t __axi_gpio_base;

typedef volatile struct {
    uint32_t gpio_data;
    uint32_t gpio_tri;
    uint32_t reserved0[69];
    uint32_t gier;
    uint32_t ipisr;
    uint32_t reserved1;
    uint32_t ipier;
} axi_gpio_t;

#endif /* AXI_GPIO_H */

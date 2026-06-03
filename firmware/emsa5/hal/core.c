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

#include "core.h"
#include "csr.h"

volatile uint32_t exception_reason[2] __attribute((section(".exc_data")));

void core_disable_interrupts(void)
{
    // set mstatus.MIE=0 (disable M mode interrupt)
    __builtin_csr_clear_bits(CSR_MSTATUS, 0x8);
}

void core_disable_external_interrupt(void)
{
    // set mie.MEIE=0 (disable M mode external interrupts)
    __builtin_csr_clear_bits(CSR_MIE, 0x800);
}

void core_disable_timer_interrupt(void)
{
    // set mie.MTIE=0 (disable M mode timer interrupts)
    __builtin_csr_clear_bits(CSR_MIE, 0x80);
}

void core_enable_interrupts(void)
{
    // set mstatus.MIE=1 (enable M mode interrupt)
    __builtin_csr_set_bits(CSR_MSTATUS, 0x8);
}

void core_enable_external_interrupt(void)
{
    // set mie.MEIE=1 (enable M mode external interrupts)
    __builtin_csr_set_bits(CSR_MIE, 0x800);
}

void core_enable_timer_interrupt(void)
{
    // set mie.MTIE=1 (enable M mode timer interrupts)
    __builtin_csr_set_bits(CSR_MIE, 0x80);
}


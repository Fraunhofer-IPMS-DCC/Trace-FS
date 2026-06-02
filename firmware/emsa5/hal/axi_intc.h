#ifndef AXI_INTC_H
#define AXI_INTC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern volatile uint32_t __axi_intc_base;

typedef volatile struct {
    uint32_t isr;
    uint32_t ipr;
    uint32_t ier;
    uint32_t iar;
    uint32_t sie;
    uint32_t cie;
    uint32_t ivr;
    uint32_t mer;
    uint32_t imr;
    uint32_t ilr;
    uint32_t reserved0[54];
    uint32_t ivar[32];
    uint32_t reserved1[32];
    uint32_t ivear[64];
} axi_intc_t;

#endif /* AXI_INTC_H */

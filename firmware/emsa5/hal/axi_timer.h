#ifndef AXI_TIMER_H
#define AXI_TIMER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern volatile uint32_t __axi_timer_base;

typedef volatile struct {
    uint32_t tcsr0;
    uint32_t tlr0;
    uint32_t tcr0;
    uint32_t reserved0;
    uint32_t tcsr1;
    uint32_t tlr1;
    uint32_t tcr1;
    uint32_t reserved1;
} axi_timer_t;

typedef union {
    struct {
        uint32_t mdt0 : 1;
        uint32_t udt0 : 1;
        uint32_t gent0 : 1;
        uint32_t capt0 : 1;
        uint32_t arht0 : 1;
        uint32_t load0 : 1;
        uint32_t enit0 : 1;
        uint32_t ent0 : 1;
        uint32_t t0int : 1;
        uint32_t pwma0 : 1;
        uint32_t enall : 1;
        uint32_t casc : 1;
        uint32_t reserved : 20;
    };
    uint32_t full_register;
} axi_timer_tcsr0_t;

#endif /* AXI_TIMER_H */

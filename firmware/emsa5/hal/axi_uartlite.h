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

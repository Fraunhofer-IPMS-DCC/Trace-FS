#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "axi_intc.h"
#include "core.h"
#include "irq.h"

volatile axi_intc_t axi_intc __attribute((section(".axi_intc")));

void irq_init (void)
{
    // enable IRQ output of INTC (MER.ME = 1)
    axi_intc.mer = 0x00000001;

    // enable hardware interrupts (MER.HIE = 1)
    // this disables the ability to generate the HW interrupts from SW
    axi_intc.mer = 0x00000003;

    irq_enable_all_interrupts();
    core_enable_external_interrupt();
}

void irq_enable_all_interrupts (void)
{
    // enable all interrupt sources (IER.INTx = 1)
    axi_intc.ier = 0x0000000f;
}

void irq_disable_all_interrupts (void)
{
    // disable all interrupt sources (IER.INTx = 0)
    axi_intc.ier = 0x00000000;
}

void irq_enable_interrupt (irq_t irq)
{
    uint32_t mask = 1 << irq;

    axi_intc.ier |= mask;
}

void irq_disable_interrupt (irq_t irq)
{
    uint32_t mask = ~(1 << irq) & 0xf;

    axi_intc.ier &= mask;
}

void irq_acknowledge_interrupt (irq_t irq)
{
    uint32_t mask = 1 << irq;

    axi_intc.iar = mask;;
}

bool irq_is_pending_interrupt (irq_t irq)
{
    uint32_t mask = 1 << irq;

    return axi_intc.ipr & mask;
}

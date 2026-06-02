#ifndef IRQ_H
#define IRQ_H

typedef enum {
    IRQ_0 = 0,
    IRQ_1 = 1,
    IRQ_2 = 2,
    IRQ_3 = 3,
} irq_t;

void irq_init (void);

void irq_enable_all_interrupts (void);
void irq_disable_all_interrupts (void);

void irq_enable_interrupt (irq_t irq);
void irq_disable_interrupt (irq_t irq);
void irq_acknowledge_interrupt (irq_t irq);
bool irq_is_pending_interrupt (irq_t irq);

#endif /* IRQ_H */

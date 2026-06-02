#ifndef GPIO_H
#define GPIO_H

void gpio_init (void);

void gpio_acknowledge_interrupt (void);
void gpio_enable_interrupt (void);
void gpio_disable_interrut (void);

#endif /* GPIO_H */

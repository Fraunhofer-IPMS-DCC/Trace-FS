/**
  * Hardware Abstraction Layer for TRISTAN TRACE_FS demonstrator.
  *
  * (c) 2025-2026 AbsInt Angewandte Informatik GmbH
  *
  * Author:  Simon Wegener
  * License: MIT
  */

#ifndef GPIO_H
#define GPIO_H

void gpio_init (void);

void gpio_acknowledge_interrupt (void);
void gpio_enable_interrupt (void);
void gpio_disable_interrut (void);

#endif /* GPIO_H */

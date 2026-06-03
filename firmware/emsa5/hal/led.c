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

#include "axi_gpio.h"
#include "led.h"

void led_init (void)
{
    led_toggle(LED_BLOCK_0, OFF);
    led_toggle(LED_BLOCK_1, OFF);
}

void led_toggle (led_t led, led_status_t status)
{
    uint8_t mask_0 = 0x00;
    uint8_t mask_1 = 0x00;

    switch (led) {
        case LED_0:
        case LED_1:
        case LED_2:
        case LED_3:
            mask_1 = 1 << led;
            break;

        case LED_BLOCK_0:
            mask_0 = 0xc0;
            break;

        case LED_BLOCK_1:
            mask_1 = 0x0f;
            break;

        case LED_UF_1:
        case LED_UF_2:
            mask_0 = 1 << led;
            break;
    }

    if (mask_0) {
        volatile uint8_t * gpio_ptr = ((uint8_t *)&__axi_gpio_base) + 3;

        *gpio_ptr = (*gpio_ptr & ~mask_0) | (status & mask_0);
    }
    if (mask_1) {
        volatile uint8_t * gpio_ptr = ((uint8_t *)&__axi_gpio_base);

        *gpio_ptr = (*gpio_ptr & ~mask_1) | (status & mask_1);
    }
}

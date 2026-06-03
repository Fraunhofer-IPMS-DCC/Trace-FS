/**
  * Hardware Abstraction Layer for TRISTAN TRACE_FS demonstrator.
  *
  * (c) 2025-2026 AbsInt Angewandte Informatik GmbH
  *
  * Author:  Simon Wegener
  * License: MIT
  */

#ifndef LED_H
#define LED_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    LED_0 = 0,
    LED_1 = 1,
    LED_2 = 2,
    LED_3 = 3,
    LED_BLOCK_0 = 4,
    LED_BLOCK_1 = 5,
    LED_UF_1 = 6,
    LED_UF_2 = 7,
} led_t;

typedef enum {
    ON = 0xff,
    OFF = 0x00,
} led_status_t;

void led_init (void);
void led_toggle (led_t led, led_status_t status);

#endif /* LED_H */

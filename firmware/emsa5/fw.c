#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "core.h"
#include "heartbeat.h"
#include "irq.h"
#include "gpio.h"
#include "led.h"
#include "scheduler.h"
#include "timer.h"

void blinkenlights (void)
{
    while (true) {
        uint32_t cycles = __builtin_csr_read(CSR_MCYCLE);
        uint32_t led_bits = ((cycles & 0x03000000) << 6) >> 24;

        if (led_bits) {
            led_toggle(LED_BLOCK_0, led_bits);
        }
    }
}

void hw_init (void)
{
    irq_init();
    gpio_init();
    led_init();
    timer_init();
}

int main (void)
{
    heartbeat_init();
    heartbeat_beat();

    hw_init();
    heartbeat_beat();

    core_enable_interrupts();
    heartbeat_beat();

    scheduler_init();
    scheduler_run();

    blinkenlights();

    return 0;
}


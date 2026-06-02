#include "axi_gpio.h"
#include "gpio.h"

volatile axi_gpio_t axi_gpio __attribute((section(".axi_gpio")));

void gpio_init (void)
{
    // reset GPIO state
    axi_gpio.gpio_data = 0x00000000;
    axi_gpio.gier = 0x00000000;
    axi_gpio.ipier = 0x00000000;

    gpio_acknowledge_interrupt();

    // PMOD_2 as input pins, all other pins as output
    axi_gpio.gpio_tri = 0x0000f000;

    gpio_enable_interrupt();
}

void gpio_acknowledge_interrupt (void)
{
    if (axi_gpio.ipisr)
        axi_gpio.ipisr = 0x00000001;
}

void gpio_enable_interrupt (void)
{
    axi_gpio.gier = 0x80000000;
    axi_gpio.ipier = 0x00000001;
}

void gpio_disable_interrut (void)
{
    axi_gpio.ipier = 0x00000000;
}

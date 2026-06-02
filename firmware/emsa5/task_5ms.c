#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "core.h"
#include "led.h"
#include "scheduler.h"
#include "asw/scenarios_a3/scenarios_a3.h"

#define WAIT_TIME 0x1000

void task_5ms (void)
{
    task_5ms_bs_math();

    bool flag = signal != 0;

    led_toggle(flag ? LED_UF_1 : LED_UF_2, ON);
    core_busy_waiting(WAIT_TIME);
    led_toggle(flag ? LED_UF_1 : LED_UF_2, OFF);
}

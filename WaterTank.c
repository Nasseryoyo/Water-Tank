/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <Drivers/Actuators/LED/LED.h>
#include "pico/stdlib.h"

int main()
{
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);



    while (true)
    {
        pico_set_led(true);

        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        }
    return 0;
    }

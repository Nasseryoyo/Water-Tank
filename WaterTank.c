/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <pico/stdlib.h>
#include <Drivers/Actuators/LED/LED.h>
#include <Drivers/Sensors/UltraSonic/UltraSonic.h>


int main()
    {
    // Period for ultrasonic sensor needs to be at least 20ms or
    // else it may have an error at the rated 2m range
     const uint32_t us_period = 30;

    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    stdio_init_all();
    UltraSonicConfig sensor = { .trigger_pin = 2, .echo_pin = 3 }; // Example pins
    ultrasonic_init(&sensor);;

    while (true)
    {
        pico_set_led(true);
        
        float distance = ultrasonic_get_distance(&sensor);
        if (distance >= 0) {
            printf("Distance: %.2f cm\n", distance);
        } else {
            printf("Error: No echo received or timeout occurred.\n");
        }
        sleep_ms(500); // Delay for readability
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        }


    
    return 0;
    }

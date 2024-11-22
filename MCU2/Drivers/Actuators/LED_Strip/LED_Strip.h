#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include "hardware/pwm.h"

void LED_Strip_init(uint pin);

void LED_Strip_set_Color( uint8_t red, uint8_t green, uint8_t blue);
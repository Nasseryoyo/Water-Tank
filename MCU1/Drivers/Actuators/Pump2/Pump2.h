#pragma once

#include <pico/stdlib.h>

// Initialize the water pump with the specified GPIO pin
void pump2_init(uint pin);

// Turn the pump on
void pump2_on(void);

// Turn the pump off
void pump2_off(void);




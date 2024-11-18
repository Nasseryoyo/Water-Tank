#pragma once

#include <pico/stdlib.h>

// Initialize the water pump with the specified GPIO pin
void pump_init(uint pin);

// Turn the pump on
void pump_on(void);

// Turn the pump off
void pump_off(void);



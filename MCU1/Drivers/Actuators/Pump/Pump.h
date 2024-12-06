#pragma once

#include <pico/stdlib.h>

// Initialize the water pump with the specified GPIO pin
int pump_init(uint pin);

// Turn the pump on
void pump_on(int pump_id);

// Turn the pump off
void pump_off(int pump_id);

bool pump_is_on(int pump_id);



#pragma once

#include <pico/stdlib.h>

// Initializes the water level sensor
void water_level_sensor_init(uint pin);

// Reads the water level sensor
// Returns 1 if water is detected (full), 0 otherwise
int is_tank_full();


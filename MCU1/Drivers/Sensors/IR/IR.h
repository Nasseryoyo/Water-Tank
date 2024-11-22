#pragma once

#include "pico/stdlib.h"

// Function prototypes
void ir_sensor_init(uint pin);
bool ir_sensor_detect(void);


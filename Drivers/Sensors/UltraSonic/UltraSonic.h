#pragma once

#include "pico/stdlib.h"

#ifndef SPEED_OF_SOUND
// Speed of sound in air (in cm/us)
#define SPEED_OF_SOUND 0.034

#endif
// Configuration structure for the ultrasonic sensor
typedef struct {
    uint trigger_pin; // GPIO pin connected to the trigger
    uint echo_pin;    // GPIO pin connected to the echo
} UltraSonicConfig;

// Initializes the ultrasonic sensor with the given configuration
void ultrasonic_init(UltraSonicConfig *config);

// Measures the distance in centimeters
float ultrasonic_get_distance(UltraSonicConfig *config);


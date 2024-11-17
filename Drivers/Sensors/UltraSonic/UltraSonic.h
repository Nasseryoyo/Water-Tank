#pragma once

#include <pico/stdlib.h>
#include <stdio.h>

#ifndef SPEED_OF_SOUND
#define SPEED_OF_SOUND 0.0343f
#endif

#ifndef ULTRASONIC_TIMEOUT
#define ULTRASONIC_TIMEOUT 200000 // 200 ms

#endif 

// Initializes the ultrasonic sensor
void ultrasonic_sensor_init(uint trig_pin, uint echo_pin);

// Measures the distance using the ultrasonic sensor
// Returns distance in cm
float ultrasonic_get_distance();


#pragma once

#include <pico/stdlib.h>
#include <stdio.h>

#ifndef SPEED_OF_SOUND
#define SPEED_OF_SOUND 0.0343f
#endif

#ifndef ULTRASONIC_TIMEOUT
#define ULTRASONIC_TIMEOUT 200000 // 200 ms
#endif 

#ifndef ULTRASONIC_MIN_DISTANCE
#define ULTRASONIC_MIN_DISTANCE 2.0f
#endif

#ifndef ULTRASONIC_MAX_DISTANCE
#define ULTRASONIC_MAX_DISTANCE 400.0f
#endif

#ifndef ULTRASONIC_ERROR_TIMEOUT_START
#define ULTRASONIC_ERROR_TIMEOUT_START -1
#endif

#ifndef ULTRASONIC_ERROR_TIMEOUT_END
#define ULTRASONIC_ERROR_TIMEOUT_END -2
#endif

#ifndef ULTRASONIC_ERROR_OUT_OF_RANGE
#define ULTRASONIC_ERROR_OUT_OF_RANGE -3
#endif

// Initializes the ultrasonic sensor
void ultrasonic_sensor_init(uint trig_pin, uint echo_pin);

// Measures the distance using the ultrasonic sensor
// Returns distance in cm
float ultrasonic_get_distance();

// Measures the distance using the ultrasonic sensor multiple times and returns the average distance
// Returns distance in cm
float ultrasonic_get_average_distance(int num_samples);
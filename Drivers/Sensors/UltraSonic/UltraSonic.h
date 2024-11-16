#pragma once

#ifndef ULTRASONIC_H
#define ULTRASONIC_H
#include "pico/stdlib.h"


// Define the pins for the ultrasonic sensor
#define TRIG_PIN 2
#define ECHO_PIN 3

// Function to initialize the ultrasonic sensor
void ultrasonic_init(void);

// Function to get the distance measured by the ultrasonic sensor
uint16_t ultrasonic_get_distance(void);

#endif // ULTRASONIC_H
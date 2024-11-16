#pragma once

#include <pico/stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pico/stdio.h>


// Define the pins for the ultrasonic sensor
#define TRIG_PIN 8
#define ECHO_PIN 9

// Function to initialize the ultrasonic sensor
void ultrasonic_init(void);

// Function to get the distance measured by the ultrasonic sensor
uint16_t ultrasonic_get_distance(void);

#pragma once

#include <stdint.h>

// Function to initialize the ultrasonic sensor
void UltraSonic_Init(uint trigger_pin, uint echo_pin);

// Function to trigger the ultrasonic sensor
void UltraSonic_Trigger(void);

// Function to get the distance measured by the ultrasonic sensor
uint16_t UltraSonic_GetDistance(void);



#pragma once

#include "pico/stdlib.h"

#ifndef BUZZER_H
#define BUZZER_H

// Define the GPIO pin for the buzzer
#define BUZZER_PIN 15

// Initialize the buzzer
void buzzer_init(void);

// Turn on the buzzer
void buzzer_on(void);

// Turn off the buzzer
void buzzer_off(void);

#endif // BUZZER_H
#pragma once

#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Initialize the buzzer with PWM
void buzzer_init(uint pin);

// Play a tone on the buzzer with a specific frequency (Hz)
void buzzer_play_tone(uint frequency);

// Stop the buzzer
void buzzer_stop(void);
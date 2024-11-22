#include <Drivers/Actuators/Buzzer/Buzzer.h>


static uint buzzer_pin;
static uint buzzer_slice_num;

void buzzer_init(uint pin) {
    buzzer_pin = pin;

    // Set up the PWM hardware for the pin
    gpio_set_function(buzzer_pin, GPIO_FUNC_PWM);

    // Get the PWM slice number for the pin
    buzzer_slice_num = pwm_gpio_to_slice_num(buzzer_pin);

    // Configure the PWM slice
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f); // Set clock divider for a base frequency
    pwm_init(buzzer_slice_num, &config, true);

    // Initially set the duty cycle to 0 (buzzer off)
    pwm_set_gpio_level(buzzer_pin, 0);
}



void buzzer_play_tone(uint frequency) {
    if (frequency == 0) {
        buzzer_stop();
        return;
    }

    // Calculate the PWM wrap value for the desired frequency
    uint32_t clock_freq = 125000000; // RP2040 clock frequency (125 MHz)
    uint16_t wrap = clock_freq / (4 * frequency) - 1; // Divider 4.0f from init

    // Update the PWM slice
    pwm_set_wrap(buzzer_slice_num, wrap);

    // Set duty cycle to 50% for a square wave
    uint16_t level = wrap / 2;
    pwm_set_gpio_level(buzzer_pin, level);

    // Enable the PWM output
    pwm_set_enabled(buzzer_slice_num, true);
}

void buzzer_stop(void) {
    // Set the duty cycle to 0 to stop the buzzer
    pwm_set_gpio_level(buzzer_pin, 0);
    pwm_set_enabled(buzzer_slice_num, false);
}
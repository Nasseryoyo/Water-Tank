#include <Drivers/Sensors/UltraSonic/UltraSonic.h>

void ultrasonic_init(UltraSonicConfig *config) {
    gpio_init(config->trigger_pin);
    gpio_set_dir(config->trigger_pin, GPIO_OUT);
    gpio_put(config->trigger_pin, 0); // Ensure the trigger is low

    gpio_init(config->echo_pin);
    gpio_set_dir(config->echo_pin, GPIO_IN);
}

float ultrasonic_get_distance(UltraSonicConfig *config) {
    // Send a 10 microsecond pulse to the trigger pin
    gpio_put(config->trigger_pin, 1);
    sleep_us(10);
    gpio_put(config->trigger_pin, 0);

    // Wait for the echo pin to go HIGH
    uint64_t start_time = time_us_64();
    while (gpio_get(config->echo_pin) == 0) {
        if (time_us_64() - start_time > 50000) { // Timeout after 50ms
            return -1; // No echo detected
        }
    }
    
    // Record the time when the echo starts
    start_time = time_us_64();

    // Wait for the echo pin to go LOW
    while (gpio_get(config->echo_pin) == 1) {
        if (time_us_64() - start_time > 50000) { // Timeout after 50ms
            return -1; // Echo took too long
        }
    }
    
    // Record the time when the echo ends
    uint64_t end_time = time_us_64();

    // Calculate the duration of the echo pulse
    uint64_t duration = end_time - start_time;

    // Calculate the distance in cm
    float distance = (duration * SPEED_OF_SOUND) / 2.0f;

    return distance;
}

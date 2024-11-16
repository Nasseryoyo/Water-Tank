#include "UltraSonic.h"
#include <time.h>
#include <unistd.h>
#include <pico/stdio.h>

#define SPEED_OF_SOUND 0.0343    // Speed of sound in cm/us
#define MIN_DISTANCE_THRESHOLD 2 // Minimum valid distance in cm

void ultrasonic_init(void)
{
    // Initialize the trigger pin
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_put(TRIG_PIN, 0); // Ensure the trigger is low

    // Initialize the echo pin
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
}

uint16_t ultrasonic_get_distance(void)
{
    // Send a 10 microsecond pulse to the trigger pin
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    // Wait for the echo pin to go HIGH
    uint64_t start_time = time_us_64();
    while (gpio_get(ECHO_PIN) == 0)
    {
        if (time_us_64() - start_time > 50000)
        {              // Timeout after 50ms
            return -1; // No echo detected
        }
    }

    // Record the time when the echo starts
    start_time = time_us_64();

    // Wait for the echo pin to go LOW
    while (gpio_get(ECHO_PIN) == 1)
    {
        if (time_us_64() - start_time > 50000)
        {              // Timeout after 50ms
            return -1; // Echo took too long
        }
    }

    // Record the time when the echo ends
    uint64_t end_time = time_us_64();

    // Calculate the duration of the echo pulse
    uint64_t duration = end_time - start_time;

    // Calculate the distance in cm
    float distance = (duration * SPEED_OF_SOUND) / 2.0f;

    // Apply the minimum distance threshold
    if (distance < MIN_DISTANCE_THRESHOLD)
    {
        return -1; // Distance is too small, consider it invalid
    }

    return (uint16_t)distance;
}
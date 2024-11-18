#include <Drivers/Sensors/UltraSonic/UltraSonic.h>


static uint trig_pin, echo_pin;

void ultrasonic_sensor_init(uint trig, uint echo) {
    trig_pin = trig;
    echo_pin = echo;

    gpio_init(trig_pin);
    gpio_set_dir(trig_pin, GPIO_OUT);
    gpio_put(trig_pin, 0);

    gpio_init(echo_pin);
    gpio_set_dir(echo_pin, GPIO_IN);
}

float ultrasonic_get_distance() {
    // Send trigger pulse
    gpio_put(trig_pin, 1);
    sleep_us(10); // 10 microseconds pulse
    gpio_put(trig_pin, 0);

    // Wait for echo pin to go HIGH
    uint32_t start_time = time_us_32();
    while (gpio_get(echo_pin) == 0) {
        if ((time_us_32() - start_time) > ULTRASONIC_TIMEOUT) { // Timeout: 200 ms
            printf("[DEBUG] Ultrasonic sensor timeout: Echo pin never went HIGH.\n");
            return ULTRASONIC_ERROR_TIMEOUT_START; // Indicate error
        }
    }

    // Record the time when echo pin goes HIGH
    uint32_t echo_start_time = time_us_32();

    // Wait for echo pin to go LOW
    while (gpio_get(echo_pin) == 1) {
        if ((time_us_32() - echo_start_time) > ULTRASONIC_TIMEOUT) { // Timeout: 200 ms
            printf("[DEBUG] Ultrasonic sensor timeout: Echo pin never went LOW.\n");
            return ULTRASONIC_ERROR_TIMEOUT_END; // Indicate error
        }
    }

    // Record the time when echo pin goes LOW
    uint32_t echo_end_time = time_us_32();

    // Calculate duration and distance
    float duration = (float)(echo_end_time - echo_start_time);
    float distance = (duration / 2.0f) * SPEED_OF_SOUND; // Speed of sound in cm/us

    // Validate distance
    if (distance < ULTRASONIC_MIN_DISTANCE || distance > ULTRASONIC_MAX_DISTANCE ) { // HC-SR04 range: 2 cm - 400 cm
        printf("[DEBUG] Ultrasonic sensor error: Measured distance %.2f cm is out of range.\n", distance);
        return ULTRASONIC_ERROR_OUT_OF_RANGE; // Indicate error
    }

    return distance;
}

#include <Drivers/Actuators/LED_Strip/LED_Strip.h>

static uint PIN;

void LED_Strip_init(uint pin) {
    PIN = pin;
    // Set up PWM on the pin
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(slice_num, 124); // Set wrap value for frequency ~800 kHz
    pwm_set_enabled(slice_num, true);
}

void send_bit(bool bit) {
    if (bit) {
        // Logic 1: ~850 ns HIGH, ~400 ns LOW
        pwm_set_gpio_level(PIN, 6); // ~70% duty cycle
        sleep_us(1);                   // Hold HIGH for ~850 ns
        pwm_set_gpio_level(PIN, 2); // ~30% duty cycle
        sleep_us(1);                   // Hold LOW for ~400 ns
    } else {
        // Logic 0: ~400 ns HIGH, ~850 ns LOW
        pwm_set_gpio_level(PIN, 2); // ~30% duty cycle
        sleep_us(1);                   // Hold HIGH for ~400 ns
        pwm_set_gpio_level(PIN, 6); // ~70% duty cycle
        sleep_us(1);                   // Hold LOW for ~850 ns
    }
}

void LED_Strip_set_Color( uint8_t red, uint8_t green, uint8_t blue) {
    // Send 24 bits (GRB format)
    for (int i = 7; i >= 0; i--) send_bit((green >> i) & 1);
    for (int i = 7; i >= 0; i--) send_bit((red >> i) & 1);
    for (int i = 7; i >= 0; i--) send_bit((blue >> i) & 1);
}
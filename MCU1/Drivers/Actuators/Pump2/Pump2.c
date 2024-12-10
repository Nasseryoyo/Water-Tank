#include <Drivers/Actuators/Pump/Pump.h>

// Static variable to hold the pump control pin
static uint pump2_pin;

void pump2_init(uint pin) {
    pump2_pin = pin;
    gpio_init(pump2_pin);
    gpio_set_dir(pump2_pin, GPIO_OUT);
    gpio_put(pump2_pin, 0); // Ensure pump is off initially
}

void pump2_on(void) {
    gpio_put(pump2_pin, 1);
}

void pump2_off(void) {
    gpio_put(pump2_pin, 0);
}

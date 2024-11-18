#include <Drivers/Actuators/Pump/Pump.h>

// Static variable to hold the pump control pin
static uint pump_pin;

void pump_init(uint pin) {
    pump_pin = pin;
    gpio_init(pump_pin);
    gpio_set_dir(pump_pin, GPIO_OUT);
    gpio_put(pump_pin, 0); // Ensure pump is off initially
}

void pump_on(void) {
    gpio_put(pump_pin, 1);
}

void pump_off(void) {
    gpio_put(pump_pin, 0);
}

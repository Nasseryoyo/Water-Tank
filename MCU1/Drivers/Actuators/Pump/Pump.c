#include <Drivers/Actuators/Pump/Pump.h>

#define MAX_PUMPS 10 // Maximum number of pumps

// Struct to hold pump configuration
typedef struct {
    uint pump_pin;
    bool is_on;
} Pump;

// Array to store pump configurations
static Pump pumps[MAX_PUMPS];
static int pump_count = 0;

// Initialize a pump with the specified GPIO pin
int pump_init(uint pin) {
    if (pump_count >= MAX_PUMPS) {
        return -1; // Error: Exceeded maximum pump count
    }

    pumps[pump_count].pump_pin = pin;
    pumps[pump_count].is_on = false;

    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, 0); // Ensure the pump is off initially

    return pump_count++; // Return the pump ID
}

// Turn a specific pump on
void pump_on(int pump_id) {
    if (pump_id < 0 || pump_id >= pump_count) {
        return; // Invalid pump ID
    }

    gpio_put(pumps[pump_id].pump_pin, 1);
    pumps[pump_id].is_on = true;
}

// Turn a specific pump off
void pump_off(int pump_id) {
    if (pump_id < 0 || pump_id >= pump_count) {
        return; // Invalid pump ID
    }

    gpio_put(pumps[pump_id].pump_pin, 0);
    pumps[pump_id].is_on = false;
}

// Check if a specific pump is on
bool pump_is_on(int pump_id) {
    if (pump_id < 0 || pump_id >= pump_count) {
        return false; // Invalid pump ID
    }

    return pumps[pump_id].is_on;
}

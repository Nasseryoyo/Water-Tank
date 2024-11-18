#include <Drivers/Sensors/WaterLevel/WaterLevel.h>

// Pin for the water level sensor
static uint water_level_pin;

void water_level_sensor_init(uint pin) {
    water_level_pin = pin;
    gpio_init(water_level_pin);
    gpio_set_dir(water_level_pin, GPIO_IN);
}

int is_tank_full() {
    return gpio_get(water_level_pin);
}

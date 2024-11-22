#include <Drivers/Sensors/IR/IR.h>

static uint ir_pin;

void ir_sensor_init(uint pin) {
    ir_pin = pin;

    gpio_init(ir_pin);
    gpio_set_dir(ir_pin, GPIO_IN);

    printf("[DEBUG] IR sensor initialized: PIN=%d\n", ir_pin);
    }

bool ir_sensor_detect(void) {
    printf("[DEBUG] IR sensor state: %d \n", gpio_get(ir_pin));
    return gpio_get(ir_pin);
    }

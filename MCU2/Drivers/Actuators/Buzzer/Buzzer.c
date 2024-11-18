#include "Buzzer.h"

static uint buzzer_pin;

void buzzer_init(uint pin){
    buzzer_pin = pin;
    gpio_init(buzzer_pin);
    gpio_set_dir(buzzer_pin, GPIO_OUT);
}

void buzzer_on(void)
{
    gpio_put(buzzer_pin, 1);
}

void buzzer_off(void)
{
    gpio_put(buzzer_pin, 0);
}
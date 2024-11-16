#include "Buzzer.h"

void buzzer_init(void)
{
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
}

void buzzer_on(void)
{
    gpio_put(BUZZER_PIN, 1);
}

void buzzer_off(void)
{
    gpio_put(BUZZER_PIN, 0);
}
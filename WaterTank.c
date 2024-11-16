#include <pico/stdlib.h>
#include <Drivers/Actuators/LED/LED.h>
#include <Drivers/Actuators/LCD/LCD.h>
#include <Drivers/Actuators/Buzzer/Buzzer.h>
#include <Drivers/Actuators/LCD/LCD.h>
#include <Drivers/Sensors/UltraSonic/UltraSonic.h>

#include <pico/stdlib.h>
#include <Drivers/Actuators/LED/LED.h>
#include <Drivers/Actuators/LCD/LCD.h>
#include <Drivers/Actuators/Buzzer/Buzzer.h>
#include <Drivers/Sensors/UltraSonic/UltraSonic.h>

int main()
{
    // Period for ultrasonic sensor needs to be at least 20ms or
    // else it may have an error at the rated 2m range
    const uint32_t us_period = 30;
    buzzer_init();

    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    stdio_init_all();

    ultrasonic_init();

    while (true)
    {
        pico_set_led(true);

        uint16_t distance = ultrasonic_get_distance();
        if (distance == (uint16_t)-1)
        {
            buzzer_on();
        }
        else
        {
            buzzer_off();
        }
        sleep_ms(500); // Delay for readability
        sleep_ms(us_period);
        pico_set_led(false);
    }

    return 0;
}
int main()
{
    // Period for ultrasonic sensor needs to be at least 20ms or
    // else it may have an error at the rated 2m range
    const uint32_t us_period = 30;
    buzzer_init();

    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    stdio_init_all();

    lcd_init();
    lcd_clear();
    ultrasonic_init();

    while (true)
    {
        pico_set_led(true);

        uint16_t distance = ultrasonic_get_distance();
        lcd_clear();
        if (distance == (uint16_t)-1)
        {
            lcd_set_cursor(0, 0);
            lcd_print("No echo detected");
            buzzer_on();
        }
        else
        {
            char buffer[16];
            snprintf(buffer, sizeof(buffer), "Distance: %u cm", distance);
            lcd_set_cursor(0, 0);
            lcd_print(buffer);
            buzzer_off();
        }
        sleep_ms(500); // Delay for readability
        sleep_ms(us_period);
        pico_set_led(false);
    }

    return 0;
}
#include <stdio.h>
#include <pico/stdlib.h>
#include <Drivers/Actuators/LED/LED.h>
#include <Drivers/Actuators/LCD/LCD.h>
#include <Drivers/Actuators/Buzzer/Buzzer.h>
#include <Drivers/Sensors/UltraSonic/UltraSonic.h>
#include <Drivers/Sensors/WaterLevel/WaterLevel.h> // Add the Water Level Sensor header

// Define the pins for the water level sensor
#define WATER_LEVEL_PIN 4

// Define the pins for the ultrasonic sensor
#define TRIG_PIN 2
#define ECHO_PIN 3

int main() {
    // Period for ultrasonic sensor needs to be at least 20ms or
    // else it may have an error at the rated 2m range
    const uint32_t us_period = 30;

    // Initialize all actuators and sensors
    buzzer_init();
    lcd_init();
    lcd_clear();
    ultrasonic_sensor_init(TRIG_PIN, ECHO_PIN); // Initialize the ultrasonic sensor
    water_level_sensor_init(WATER_LEVEL_PIN); // Initialize the water level sensor


    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);


    stdio_init_all(); // Initialize stdio for debugging

 
    while (true) {
        pico_set_led(true);

        // Check if the tank is full
        if (is_tank_full()) {
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_print("Tank is Full");
            printf("Tank is Full\n");
            buzzer_off(); // No need for alarm
        } else {
            // Measure distance using ultrasonic sensor
            uint16_t distance = ultrasonic_get_distance();
            lcd_clear();

            // Check if the ultrasonic sensor has an error 
            if (distance < 0) {
                lcd_set_cursor(0, 0);
                lcd_print("No echo detected");
                printf("No echo detected\n");
                buzzer_on();
                }
            // Check if the tank is empty
            else {
                if (distance > 30) { // 30 cm is the maximum distance for the tank
                    lcd_set_cursor(0, 0);
                    lcd_print("Tank is Empty");
                    printf("Tank is Empty\n");
                    buzzer_on(); // Alarm for empty tank
                    }
                // Display the water level
                else {
                    char buffer[16];
                    snprintf(buffer, sizeof(buffer), "Water: %u cm", distance);
                    printf("%s\n", buffer);
                    lcd_set_cursor(0, 0);
                    lcd_print(buffer);
                    buzzer_off(); // No alarm
                    }
                }
            }

        sleep_ms(500); // Delay for readability
        sleep_ms(us_period);
        pico_set_led(false);
    }

    return 0;
}

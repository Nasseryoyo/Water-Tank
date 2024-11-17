#include <stdio.h>
#include <pico/stdlib.h>

// Actuator Drivers
#include <Drivers/Actuators/LED/LED.h>
#include <Drivers/Actuators/LCD/LCD.h>
#include <Drivers/Actuators/Buzzer/Buzzer.h>
#include <Drivers/Actuators/Pump/Pump.h>

// Sensor Drivers
#include <Drivers/Sensors/UltraSonic/UltraSonic.h>
#include <Drivers/Sensors/WaterLevel/WaterLevel.h>

// Define the pins for the water level sensor
#define WATER_LEVEL_PIN 4

// Define the pins for the ultrasonic sensor
#define TRIG_PIN 2
#define ECHO_PIN 3

// Define the GPIO pin for the buzzer
#define BUZZER_PIN 15

// Define the GPIO pin connected to the pump
#define PUMP_PIN 5

int main() {
    // Period for ultrasonic sensor needs to be at least 20ms or
    // else it may have an error at the rated 2m range
    const uint32_t us_period = 30;

    // Initialize actuators
    buzzer_init(BUZZER_PIN);
    pump_init(PUMP_PIN);
    lcd_init();
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    lcd_clear();

    // Initialize the Sensors
    ultrasonic_sensor_init(TRIG_PIN, ECHO_PIN); // Initialize the ultrasonic sensor
    water_level_sensor_init(WATER_LEVEL_PIN);  // Initialize the water level sensor

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
            pump_off();   // Turn off the pump
        } else {
            // Measure distance using ultrasonic sensor
            uint16_t distance = ultrasonic_get_distance();
            lcd_clear();

            // Check if the ultrasonic sensor has an error
            if (distance == (uint16_t)-1) {
                lcd_set_cursor(0, 0);
                lcd_print("No echo detected");
                printf("No echo detected\n");
                buzzer_on();
            } else {
                // Check if the tank is empty
                if (distance > 30) { // 30 cm is the maximum distance for the tank
                    lcd_set_cursor(0, 0);
                    lcd_print("Tank is Empty");
                    printf("Tank is Empty\n");
                    buzzer_on();  // Alarm for empty tank
                    pump_on();   // Turn on the pump
                } else {
                    // Display the water level
                    char buffer[16];
                    snprintf(buffer, sizeof(buffer), "Water: %u cm", distance);
                    printf("%s\n", buffer);
                    lcd_set_cursor(0, 0);
                    lcd_print(buffer);
                    buzzer_off();  // No alarm
                    pump_off();    // Turn off the pump
                }
            }
        }

        sleep_ms(500); // Delay for readability
        sleep_ms(us_period);
        pico_set_led(false);
    }

    return 0;
}

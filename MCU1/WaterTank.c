#include <stdio.h>
#include <pico/stdlib.h>
#include <time.h>

// Actuator Drivers
#include <Drivers/Actuators/LED/LED.h>
#include <Drivers/Actuators/Pump/Pump.h>

// Sensor Drivers
#include <Drivers/Sensors/UltraSonic/UltraSonic.h>
#include <Drivers/Sensors/IR/IR.h>
#include <Drivers/Sensors/WaterLevel/WaterLevel.h>

// Communication Drivers
#include <Drivers/Communication/uart_communication.h>

// Define the pins for the water level sensor
#define WATER_LEVEL_PIN 9

// Define the pins for the IR sensor
#define IR_PIN 10

// Define the pins for the ultrasonic sensor
#define TRIG_PIN 15
#define ECHO_PIN 14

// Define the GPIO pin connected to the pump
#define PUMP_PIN 18
#define PUMP_2_PIN 19

// Define the UART parameters
#define UART_BAUD_RATE 9600
#define UART_ID uart0
#define TX_PIN 0
#define RX_PIN 1


int main() {

    // Initialize actuators
    int pump_id =  pump_init(PUMP_PIN);
    int pump_2_id = pump_init(PUMP_2_PIN);

    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);

    // Initialize the Sensors
    ultrasonic_sensor_init(TRIG_PIN, ECHO_PIN);  // Initialize the ultrasonic sensor
    water_level_sensor_init(WATER_LEVEL_PIN);    // Initialize the water level sensor
    ir_sensor_init(IR_PIN);                      // Initialize the IR sensor

    stdio_usb_init();

    // Initialize UART
   uint bandRate = uart_init_config(UART_ID,TX_PIN,RX_PIN,UART_BAUD_RATE);  // Initialize UART for communication

    while (true) {
        pico_set_led(true);

        printf("UART Initialized on Baud rate : %d\n",bandRate);  // Print message via stdio

        // Check if the tank is full
        if (is_tank_full()) {
            uart_send_message("3 Water level is high \n");  // Send message via UART
            pump_off(pump_id);    // Turn off the pump
            pump_on(pump_2_id);  // Turn off the pump
            }
        else {
            // Measure distance using ultrasonic sensor
            uint16_t distance = ultrasonic_get_distance();
            // Check if the ultrasonic sensor has an error
            if (distance < 0) {
                uart_send_message("4 Ultrasonic sensor error\n");  // Send error via UART
                }
            else {
                // Check if the tank is empty
                if (distance > 15) { // 15 cm is the maximum distance for the tank
                    uart_send_message("1 Water level is low\n");  // Send message via UART
                    pump_on(pump_id);   // Turn on the pump
                    pump_off(pump_2_id); // Turn on the pump
                    }
                else {
                    uart_send_message("2 Water level is normal\n"); // Send water level via UART
                    pump_off(pump_id);    // Turn off the pump
                    pump_off(pump_2_id);  // Turn off the pump
                    }
            }
        }

        sleep_ms(1000);  // Sleep for 1 second
        pico_set_led(false);
    }

    return 0;
}

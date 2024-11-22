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

// Define the UART parameters
#define UART_BAUD_RATE 9600
#define UART_ID uart0
#define TX_PIN 0
#define RX_PIN 1


int main() {

    // Initialize actuators

    pump_init(PUMP_PIN);

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
            clock_t start_comm = clock();
            uart_send_message("3 Water level is high \n");  // Send message via UART
            clock_t end_comm = clock();
            double comm_time = (double)(end_comm - start_comm) / CLOCKS_PER_SEC;
        printf("UART Communication Time: %f seconds\n", comm_time);

            pump_off();    // Turn off the pump
        } else {
            // Measure distance using ultrasonic sensor
            clock_t start_sensor = clock();
            uint16_t distance = ultrasonic_get_distance();
            clock_t end_sensor = clock();
            double sensor_time = (double)(end_sensor - start_sensor) / CLOCKS_PER_SEC;
            printf("Distance Sensor Update Time: %f seconds\n", sensor_time);

            // Check if the ultrasonic sensor has an error
            if (distance < 0) {
                clock_t start_comm = clock();
                uart_send_message("4 Ultrasonic sensor error\n");  // Send error via UART
                clock_t end_comm = clock();
                double comm_time = (double)(end_comm - start_comm) / CLOCKS_PER_SEC;
                printf("UART Communication Time: %f seconds\n", comm_time);
                }
            else {
                // Check if the tank is empty
                if (distance > 30) { // 30 cm is the maximum distance for the tank
                    clock_t start_comm = clock();
                    uart_send_message("1 Water level is low\n");  // Send message via UART
                    clock_t end_comm = clock();
                    double comm_time = (double)(end_comm - start_comm) / CLOCKS_PER_SEC;
                    //printf("UART Communication Time: %f seconds\n", comm_time);
                    pump_on();   // Turn on the pump
                    }
                else {
                    clock_t start_comm = clock();
                    uart_send_message("2 Water level is normal\n"); // Send water level via UART
                    clock_t end_comm = clock();
                    double comm_time = (double)(end_comm - start_comm) / CLOCKS_PER_SEC;
                    //printf("UART Communication Time: %f seconds\n", comm_time);
                    pump_off();    // Turn off the pump
                }
            }
        }

        sleep_ms(1000);  // Sleep for 1 second
        pico_set_led(false);
    }

    return 0;
}

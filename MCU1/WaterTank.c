#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

// Actuator Drivers
#include <Drivers/Actuators/LED/LED.h>
#include <Drivers/Actuators/Pump/Pump.h>
#include <Drivers/Actuators/Pump2/Pump2.h>

// Sensor Drivers
#include <Drivers/Sensors/UltraSonic/UltraSonic.h>
#include <Drivers/Sensors/WaterLevel/WaterLevel.h>

// Communication Drivers
#include <Drivers/Communication/uart_communication.h>

// Sensor Pins
#define WATER_LEVEL_PIN 9
#define TRIG_PIN 15
#define ECHO_PIN 14

// UART Configuration
#define UART_BAUD_RATE 9600
#define UART_ID uart0
#define TX_PIN 0
#define RX_PIN 1

// Pump Pins
#define PUMP_PIN 18
#define PUMP_2_PIN 19

// Queue Handles
static QueueHandle_t xSensorQueue = NULL;

// Sensor Task
void sensor_task(void* pvParameters) {

        // Initialize Sensors
    ultrasonic_sensor_init(TRIG_PIN, ECHO_PIN);
    water_level_sensor_init(WATER_LEVEL_PIN);
    while (true) {
        uint8_t message_code;

        // Check Water Level Sensor
        if (is_tank_full()) {
            message_code = 3; // High water level
        } else {
            uint16_t distance = ultrasonic_get_distance();
            if (distance < 0) {
                message_code = 4; // Ultrasonic error
            } else if (distance > 15) {
                message_code = 1; // Low water level
            } else {
                message_code = 2; // Normal water level
            }
        }

        // Send message code to UART task
        xQueueSend(xSensorQueue, &message_code, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
    }
}

// Actuator Task
void actuator_task(void *pvParameters) {
    uint8_t message_code;

    // Initialize Actuators
    pump_init(PUMP_PIN);
    pump2_init(PUMP_2_PIN);

    while (true) {
        // Receive message from the queue
        if (xQueueReceive(xSensorQueue, &message_code, portMAX_DELAY)) {
            switch (message_code) {
                case 1: // Low water level
                    pump_on();
                    pump2_off();
                    pico_set_led(true);
                    break;
                case 2: // Normal water level
                    pump_off();
                    pump2_off();
                    pico_set_led(false);
                    break;
                case 3: // High water level
                    pump_off();
                    pump2_on();
                    pico_set_led(false);
                    break;
                case 4: // Ultrasonic error
                    pump_off();
                    pump2_off();
                    pico_set_led(true); // Flash LED on error
                    break;
                default:
                    break;
            }
        }
    }
}

// UART Task
void uart_task(void *pvParameters) {
    uint8_t message_code;

    // Initialize UART
    uart_init_config(UART_ID, TX_PIN, RX_PIN, UART_BAUD_RATE);

    while (true) {
        if (xQueueReceive(xSensorQueue, &message_code, portMAX_DELAY)) {
            switch (message_code) {
                case 1:
                    uart_send_message("1 Water level is low\n");
                    break;
                case 2:
                    uart_send_message("2 Water level is normal\n");
                    break;
                case 3:
                    uart_send_message("3 Water level is high\n");
                    break;
                case 4:
                    uart_send_message("4 Ultrasonic sensor error\n");
                    break;
                default:
                    break;
            }
        }
    }
}

// Main Function
int main() {
    stdio_usb_init();
    pico_led_init();



    // Create a queue
    xSensorQueue = xQueueCreate(5, sizeof(uint8_t));

    if (xSensorQueue == NULL) {
        printf("Failed to create queue\n");
        return 1;
    }

    // Create tasks
    xTaskCreate(sensor_task, "Sensor_Task", 256, NULL, 1, NULL);
    xTaskCreate(actuator_task, "Actuator_Task", 256, NULL, 1, NULL);
    xTaskCreate(uart_task, "UART_Task", 256, NULL, 1, NULL);

    // Start scheduler
    vTaskStartScheduler();

    // Main should never reach here
    while (true) {
        tight_loop_contents();
    }

    return 0;
}

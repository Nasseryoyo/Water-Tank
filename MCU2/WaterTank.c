#include <stdio.h>
#include <pico/stdlib.h>

// Actuator Drivers
#include <Drivers/Actuators/LED/LED.h>
#include <Drivers/Actuators/LCD/LCD.h>
#include <Drivers/Actuators/Buzzer/Buzzer.h>

// Communication Drivers
#include <Drivers/Communication/uart_communication.h>



// Define the GPIO pin for the buzzer
#define BUZZER_PIN 15

// Define the UART parameters
#define UART_BAUD_RATE 9600
#define UART_ID uart0
#define TX_PIN 0
#define RX_PIN 1


int main() {

    // Initialize actuators
    buzzer_init(BUZZER_PIN);
    lcd_init();
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    lcd_clear();

 
    stdio_init_all(); // Initialize stdio for debugging

    // Initialize the UART communication
    uart_init_config(UART_ID, TX_PIN, RX_PIN, UART_BAUD_RATE);

    while (true) {
        pico_set_led(true);

        // receive a message over UART
        char* received_msg = uart_receive_message();
        if (received_msg != NULL) {
            printf("Received message: %s\n", received_msg);
            }
        else {
            printf("No message received\n");
            }
        sleep_ms(1000);
        pico_set_led(false);
    }

    return 0;
}

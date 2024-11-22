#include <stdio.h>
#include <pico/stdlib.h>
#include <time.h>

// Actuator Drivers
#include <Drivers/Actuators/LED/LED.h>
#include <Drivers/Actuators/LCD/LCD.h>
#include <Drivers/Actuators/Buzzer/Buzzer.h>
#include <Drivers/Actuators/LED_Strip/LED_Strip.h>

// Communication Drivers
#include <Drivers/Communication/uart_communication.h>

// Define the GPIO pin for the buzzer
#define BUZZER_PIN 16

// Define the LED_Strip Pin
#define LED_Strip_PIN 17
// Define the GPIO pins for the LCD
#define LCD_PIN_D4 6
#define LCD_PIN_D5 7
#define LCD_PIN_D6 8
#define LCD_PIN_D7 9
#define LCD_BL_PIN 10 // For backlight control (optional)
#define LCD_PIN_RS 11
#define LCD_PIN_E 12

// Define the UART parameters
#define UART_BAUD_RATE 9600
#define UART_ID uart0
#define TX_PIN 0
#define RX_PIN 1

LCDdisplay lcd;

int main()
{
    // Initialize actuators
    buzzer_init(BUZZER_PIN);
    LED_Strip_init(LED_Strip_PIN);
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);

    stdio_usb_init(); // Initialize stdio for debugging

    // Initialize the UART communication
    uint bandRate = uart_init_config(UART_ID, TX_PIN, RX_PIN, UART_BAUD_RATE);

    // Initialize the LCD display
    LCDdisplay_init_with_bl(&lcd,
                            LCD_PIN_D4,
                            LCD_PIN_D5,
                            LCD_PIN_D6,
                            LCD_PIN_D7,
                            LCD_PIN_RS,
                            LCD_PIN_E,
                            LCD_BL_PIN,
                            16, 2); // 16x2 LCD

    // Initialize the LCD display
    LCDdisplay_init_display(&lcd);

    LCDdisplay_init_display(&lcd);

    while (true) {
        pico_set_led(true);

        printf("UART Initialized on Baud rate : %d\n",bandRate);  // Print message via stdio

        // Clear the LCD and display a welcome message
        LCDdisplay_clear(&lcd);
        LCDdisplay_print(&lcd, "Waiting for UART");

        // receive a message over UART
        clock_t start = clock();
        char *received_msg = uart_receive_message();
        clock_t end = clock();
        printf("Time taken to receive message: %f\n", (double)(end - start) / CLOCKS_PER_SEC);
        if  (received_msg != NULL)
        {
            printf("Received message: %s\n", received_msg);

            // Clear LCD and display appropriate messages
            LCDdisplay_clear(&lcd);

            switch (received_msg[0])
            {
            case '1':
                LCDdisplay_print(&lcd, "Water level:");
                LCDdisplay_goto_pos(&lcd, 0, 1);
                LCDdisplay_print(&lcd, "Low");
                printf("Water level is low\n");
                buzzer_play_tone(500); // Medium frequency
                break;
            case '2':
                LCDdisplay_print(&lcd, "Water level:");
                LCDdisplay_goto_pos(&lcd, 0, 1);
                LCDdisplay_print(&lcd, "Normal");
                printf("Water level is normal\n");
                buzzer_stop();
                break;
            case '3':
                LCDdisplay_print(&lcd, "Water level:");
                LCDdisplay_goto_pos(&lcd, 0, 1);
                LCDdisplay_print(&lcd, "High");
                printf("Water level is high\n");
                buzzer_play_tone(2000); // High frequency
                break;
            default:
                LCDdisplay_print(&lcd, "Invalid");
                LCDdisplay_goto_pos(&lcd, 0, 1);
                LCDdisplay_print(&lcd, "message");
                printf("Invalid message\n");
                buzzer_play_tone(100); // Low frequency for error
                break;
            }
        }
        
        sleep_ms(1000);
        pico_set_led(false);
    }

    return 0;
}
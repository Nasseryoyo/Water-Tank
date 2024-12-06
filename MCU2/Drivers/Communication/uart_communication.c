#include <Drivers/Communication/uart_communication.h>

// Values for UART communication
static uart_inst_t* UART_ID;
static uint TX_PIN;
static uint RX_PIN;
static uint UART_BAUD_RATE;

// Circular buffers
#define BUFFER_SIZE 256
static char rx_buffer[BUFFER_SIZE];  // RX buffer
static char tx_buffer[BUFFER_SIZE];  // TX buffer

// Buffer indices and flags
static volatile int rx_write_index = 0;
static volatile int rx_read_index = 0;
static volatile int tx_write_index = 0;
static volatile int tx_read_index = 0;
static volatile bool tx_in_progress = false;

// Static buffer to hold the latest message
static char latest_message[BUFFER_SIZE] = {0};
static volatile bool message_ready = false;

// RX Interrupt Handler
void uart_rx_irq_handler() {
    while (uart_is_readable(UART_ID)) {
        char c = uart_getc(UART_ID);

        // Check if the buffer is full
        int next_write_index = (rx_write_index + 1) % BUFFER_SIZE;
        if (next_write_index == rx_read_index) {
            // Reset the buffer if full
            rx_write_index = 0;
            rx_read_index = 0;
            printf("RX buffer full. Resetting buffer.\n");
        }

        // Store the received character in the RX buffer
        rx_buffer[rx_write_index] = c;
        rx_write_index = next_write_index;

        // Check for end-of-message character
        if (c == '\n' || c == '\r') {
            int index = 0;

            // Extract the complete message
            while (rx_read_index != rx_write_index) {
                char temp_char = rx_buffer[rx_read_index];
                rx_read_index = (rx_read_index + 1) % BUFFER_SIZE;

                if (temp_char == '\n' || temp_char == '\r') {
                    break;
                }

                latest_message[index++] = temp_char;
                if (index >= sizeof(latest_message) - 1) {
                    break;
                }
            }

            latest_message[index] = '\0'; // Null-terminate the message
            message_ready = true;         // Mark a new message as ready
        }
    }
}

// TX Interrupt Handler
void uart_tx_irq_handler() {
    while (uart_is_writable(UART_ID)) {
        if (tx_read_index != tx_write_index) {
            uart_putc(UART_ID, tx_buffer[tx_read_index]);
            tx_read_index = (tx_read_index + 1) % BUFFER_SIZE;
        } else {
            tx_in_progress = false; // No more data to send
            uart_set_irq_enables(UART_ID, true, false); // Disable TX interrupt
            break;
        }
    }
}

// Initialize UART with RX and TX interrupt handlers
uint uart_init_config(uart_inst_t* uart_id, uint pin_tx, uint pin_rx, uint baud_rate) {
    UART_ID = uart_id;
    TX_PIN = pin_tx;
    RX_PIN = pin_rx;
    UART_BAUD_RATE = baud_rate;

    UART_BAUD_RATE = uart_init(UART_ID, UART_BAUD_RATE);
    gpio_set_function(TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX_PIN, GPIO_FUNC_UART);
    gpio_pull_up(RX_PIN);
    uart_set_fifo_enabled(UART_ID, true);

        // Attach interrupt handlers
    if (UART_ID == uart0) {
        irq_set_exclusive_handler(UART0_IRQ, uart_rx_irq_handler); // RX and TX combined handler for UART0
        irq_set_enabled(UART0_IRQ, true);
    } else if (UART_ID == uart1) {
        irq_set_exclusive_handler(UART1_IRQ, uart_rx_irq_handler); // RX and TX combined handler for UART1
        irq_set_enabled(UART1_IRQ, true);
    }

    uart_set_irq_enables(UART_ID, true, true); // Enable RX and TX interrupts

    return UART_BAUD_RATE;
}

// Send a message using the TX buffer
void uart_send_message(char* message) {
    while (*message) {
        int next_write_index = (tx_write_index + 1) % BUFFER_SIZE;

        // Check for buffer full condition
        if (next_write_index == tx_read_index) {
            // Reset the buffer if full
            tx_write_index = 0;
            tx_read_index = 0;
            tx_in_progress = false;
            printf("TX buffer full. Resetting buffer.\n");
        }

        // Add the character to the TX buffer
        tx_buffer[tx_write_index] = *message;
        tx_write_index = next_write_index;
        message++;
    }

    if (!tx_in_progress) {
        tx_in_progress = true;
        uart_set_irq_enables(UART_ID, true, true); // Enable TX interrupt
    }
}

// Retrieve the latest message
char* uart_receive_message() {
    if (message_ready) {
        message_ready = false; // Mark the message as consumed
        return latest_message;
    }
    return NULL; // No new message is available
}

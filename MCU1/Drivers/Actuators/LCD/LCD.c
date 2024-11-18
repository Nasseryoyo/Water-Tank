#include <Drivers/Actuators/LCD/LCD.h>


// Helper function to send a nibble to the LCD
static void lcd_send_nibble(uint8_t nibble)
{
    gpio_put(LCD_D4, (nibble >> 0) & 0x01);
    gpio_put(LCD_D5, (nibble >> 1) & 0x01);
    gpio_put(LCD_D6, (nibble >> 2) & 0x01);
    gpio_put(LCD_D7, (nibble >> 3) & 0x01);

    gpio_put(LCD_EN, 1);
    sleep_us(1); // Enable pulse must be >450ns
    gpio_put(LCD_EN, 0);
    sleep_us(37); // Commands need >37us to settle
}

// Function to send a command to the LCD
void lcd_send_command(uint8_t command)
{
    gpio_put(LCD_RS, 0); // RS low for command
    lcd_send_nibble(command >> 4);
    lcd_send_nibble(command & 0x0F);
}

// Function to send data to the LCD
void lcd_send_data(uint8_t data)
{
    gpio_put(LCD_RS, 1); // RS high for data
    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data & 0x0F);
}

// Function to initialize the LCD
void lcd_init(void)
{
    // Initialize GPIO pins
    gpio_init(LCD_RS);
    gpio_set_dir(LCD_RS, GPIO_OUT);
    gpio_init(LCD_EN);
    gpio_set_dir(LCD_EN, GPIO_OUT);
    gpio_init(LCD_D4);
    gpio_set_dir(LCD_D4, GPIO_OUT);
    gpio_init(LCD_D5);
    gpio_set_dir(LCD_D5, GPIO_OUT);
    gpio_init(LCD_D6);
    gpio_set_dir(LCD_D6, GPIO_OUT);
    gpio_init(LCD_D7);
    gpio_set_dir(LCD_D7, GPIO_OUT);

    // Wait for LCD to power up
    sleep_ms(50);

    // Initialize LCD in 4-bit mode
    lcd_send_command(0x03);
    sleep_ms(5);
    lcd_send_command(0x03);
    sleep_us(150);
    lcd_send_command(0x03);
    lcd_send_command(0x02);

    // Function set: 4-bit mode, 2 lines, 5x8 dots
    lcd_send_command(LCD_FUNCTION_SET | 0x08);

    // Display control: display on, cursor off, blink off
    lcd_send_command(LCD_DISPLAY_CONTROL | 0x04);

    // Clear display
    lcd_send_command(LCD_CLEAR_DISPLAY);
    sleep_ms(2);

    // Entry mode set: increment cursor, no shift
    lcd_send_command(LCD_ENTRY_MODE_SET | 0x02);
}

// Function to clear the LCD display
void lcd_clear(void)
{
    lcd_send_command(LCD_CLEAR_DISPLAY);
    sleep_ms(2); // Clear command needs >1.52ms to settle
}

// Function to set the cursor position
void lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t address = (row == 0) ? col : (0x40 + col);
    lcd_send_command(LCD_SET_DDRAM_ADDR | address);
}

// Function to print a string on the LCD
void lcd_print(const char *str)
{
    while (*str)
    {
        lcd_send_data(*str++);
    }
}
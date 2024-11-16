#pragma once

#include <pico/stdlib.h>

// Define the pins for the LCD
#define LCD_RS 2
#define LCD_EN 3
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

// LCD commands
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_RETURN_HOME 0x02
#define LCD_ENTRY_MODE_SET 0x04
#define LCD_DISPLAY_CONTROL 0x08
#define LCD_CURSOR_SHIFT 0x10
#define LCD_FUNCTION_SET 0x20
#define LCD_SET_CGRAM_ADDR 0x40
#define LCD_SET_DDRAM_ADDR 0x80

static void lcd_send_nibble(uint8_t nibble);

void lcd_send_command(uint8_t command);

void lcd_send_data(uint8_t data);

void lcd_init(void);

void lcd_clear(void);

void lcd_set_cursor(uint8_t row, uint8_t col);

void lcd_print(const char* str);


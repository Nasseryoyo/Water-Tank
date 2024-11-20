#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include "hardware/pwm.h"

// Pin positions in LCDpins array
#define RS 4
#define E 5
// Pin values
#define HIGH 1
#define LOW 0
// LCD pin RS meaning
#define COMMAND 0
#define DATA 1

typedef struct {
    uint32_t LCDpins[6];
    uint32_t LCDmask;
    uint32_t LCDmask_c;
    uint32_t bl_pwm_pin;
    int no_chars;
    int no_lines;
    bool cursor_status[2];
} LCDdisplay;

// Function prototypes
void LCDdisplay_init(LCDdisplay* lcd, int bit4_pin, int bit5_pin, int bit6_pin, int bit7_pin, int rs_pin, int e_pin, int width, int depth);
void LCDdisplay_init_with_bl(LCDdisplay* lcd, int bit4_pin, int bit5_pin, int bit6_pin, int bit7_pin, int rs_pin, int e_pin, int bl_pin, int width, int depth);
void LCDdisplay_clear(LCDdisplay* lcd);
void LCDdisplay_cursor_off(LCDdisplay* lcd);
void LCDdisplay_cursor_on(LCDdisplay* lcd);
void LCDdisplay_cursor_on_blink(LCDdisplay* lcd, bool blink);
void LCDdisplay_display_off(LCDdisplay* lcd);
void LCDdisplay_display_on(LCDdisplay* lcd);
void LCDdisplay_set_backlight(LCDdisplay* lcd, int brightness);
void LCDdisplay_init_display(LCDdisplay* lcd);
void LCDdisplay_goto_pos(LCDdisplay* lcd, int pos_i, int line);
void LCDdisplay_print(LCDdisplay* lcd, const char* str);
void LCDdisplay_print_wrapped(LCDdisplay* lcd, const char* str);
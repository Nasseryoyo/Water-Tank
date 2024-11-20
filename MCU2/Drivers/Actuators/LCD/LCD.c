#include <Drivers/Actuators/LCD/LCD.h>

uint32_t pin_values_to_mask(LCDdisplay* lcd, uint32_t raw_bits[], int length) {
    uint32_t result = 0;
    uint32_t pinArray[32] = {0};
    for (int i = 0; i < length; i++) {
        pinArray[lcd->LCDpins[i]] = raw_bits[i];
    }
    for (int i = 0; i < 32; i++) {
        result = result << 1;
        result += pinArray[31-i];
    }
    return result;
}

void uint_into_8bits(uint32_t raw_bits[], uint32_t one_byte) {
    for (int i = 0; i < 8; i++) {
        raw_bits[7-i] = one_byte % 2;
        one_byte = one_byte >> 1;
    }
}

void init_pwm_pin(LCDdisplay* lcd, uint32_t pin) {
    lcd->bl_pwm_pin = pin;
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 500.f);
    pwm_config_set_wrap(&config, 100);
    pwm_init(slice_num, &config, true);
}

void send_raw_data_one_cycle(LCDdisplay* lcd, uint32_t raw_bits[]) {
    uint32_t bit_value = pin_values_to_mask(lcd, raw_bits, 5);
    gpio_put_masked(lcd->LCDmask, bit_value);
    gpio_put(lcd->LCDpins[E], HIGH);
    sleep_ms(5);
    gpio_put(lcd->LCDpins[E], LOW);
    sleep_ms(5);
}

void send_full_byte(LCDdisplay* lcd, uint32_t rs, uint32_t databits[]) {
    uint32_t rawbits[5];
    rawbits[4] = rs;
    for (int i = 0; i < 4; i++) {
        rawbits[i] = databits[i];
    }
    send_raw_data_one_cycle(lcd, rawbits);
    for (int i = 0; i < 4; i++) {
        rawbits[i] = databits[i+4];
    }
    send_raw_data_one_cycle(lcd, rawbits);
}

void LCDdisplay_init(LCDdisplay* lcd, int bit4_pin, int bit5_pin, int bit6_pin, int bit7_pin, int rs_pin, int e_pin, int width, int depth) {
    lcd->LCDpins[0] = bit7_pin;
    lcd->LCDpins[1] = bit6_pin;
    lcd->LCDpins[2] = bit5_pin;
    lcd->LCDpins[3] = bit4_pin;
    lcd->LCDpins[4] = rs_pin;
    lcd->LCDpins[5] = e_pin;
    lcd->bl_pwm_pin = 255;
    lcd->no_chars = width;
    lcd->no_lines = depth;
}

void LCDdisplay_init_with_bl(LCDdisplay* lcd, int bit4_pin, int bit5_pin, int bit6_pin, int bit7_pin, int rs_pin, int e_pin, int bl_pin, int width, int depth) {
    LCDdisplay_init(lcd, bit4_pin, bit5_pin, bit6_pin, bit7_pin, rs_pin, e_pin, width, depth);
    lcd->bl_pwm_pin = bl_pin;
}

void LCDdisplay_clear(LCDdisplay* lcd) {
    uint32_t clear_display[8] = {0,0,0,0,0,0,0,1};
    send_full_byte(lcd, COMMAND, clear_display);
    sleep_ms(10);
}

void LCDdisplay_cursor_off(LCDdisplay* lcd) {
    uint32_t no_cursor[8] = {0,0,0,0,1,1,0,0};
    send_full_byte(lcd, COMMAND, no_cursor);
    lcd->cursor_status[0] = false;
    lcd->cursor_status[1] = false;
}

void LCDdisplay_cursor_on(LCDdisplay* lcd) {
    uint32_t command_cursor[8] = {0,0,0,0,1,1,1,1};
    send_full_byte(lcd, COMMAND, command_cursor);
    lcd->cursor_status[0] = true;
    lcd->cursor_status[1] = true;
}

void LCDdisplay_cursor_on_blink(LCDdisplay* lcd, bool blink) {
    uint32_t command_cursor[8] = {0,0,0,0,1,1,1,0};
    if (blink) command_cursor[7] = 1;
    send_full_byte(lcd, COMMAND, command_cursor);
    lcd->cursor_status[0] = true;
    lcd->cursor_status[1] = command_cursor[7];
}

void LCDdisplay_display_off(LCDdisplay* lcd) {
    uint32_t command_display[8] = {0,0,0,0,1,0,0,0};
    command_display[7] = lcd->cursor_status[1];
    command_display[6] = lcd->cursor_status[0];
    send_full_byte(lcd, COMMAND, command_display);
}

void LCDdisplay_display_on(LCDdisplay* lcd) {
    uint32_t command_display[8] = {0,0,0,0,1,1,0,0};
    command_display[7] = lcd->cursor_status[1];
    command_display[6] = lcd->cursor_status[0];
    send_full_byte(lcd, COMMAND, command_display);
}

void LCDdisplay_set_backlight(LCDdisplay* lcd, int brightness) {
    if (lcd->bl_pwm_pin < 30) {
        pwm_set_gpio_level(lcd->bl_pwm_pin, brightness);
    }
}

void LCDdisplay_init_display(LCDdisplay* lcd) {
    uint32_t all_ones[6] = {1,1,1,1,1,1};
    uint32_t set_function_8[5] = {0,0,1,1,0};
    uint32_t set_function_4a[5] = {0,0,1,0,0};
    uint32_t set_function_4[8] = {0,0,1,0,0,0,0,0};
    uint32_t cursor_set[8] = {0,0,0,0,0,1,1,0};
    uint32_t display_prop_set[8] = {0,0,0,0,1,1,0,0};

    lcd->LCDmask_c = pin_values_to_mask(lcd, all_ones, 6);
    lcd->LCDmask = pin_values_to_mask(lcd, all_ones, 5);
    gpio_init_mask(lcd->LCDmask_c);
    gpio_set_dir_out_masked(lcd->LCDmask_c);
    gpio_clr_mask(lcd->LCDmask_c);

    if (lcd->no_lines == 2) {
        set_function_4[4] = 1;
    }
    send_raw_data_one_cycle(lcd, set_function_8);
    send_raw_data_one_cycle(lcd, set_function_8);
    send_raw_data_one_cycle(lcd, set_function_8);
    send_raw_data_one_cycle(lcd, set_function_4a);

    send_full_byte(lcd, COMMAND, set_function_4);
    send_full_byte(lcd, COMMAND, cursor_set);
    send_full_byte(lcd, COMMAND, display_prop_set);
    LCDdisplay_clear(lcd);

    if (lcd->bl_pwm_pin < 30) {
        init_pwm_pin(lcd, lcd->bl_pwm_pin);
    }

    lcd->cursor_status[0] = false;
    lcd->cursor_status[1] = false;
}

void LCDdisplay_goto_pos(LCDdisplay* lcd, int pos_i, int line) {
    uint32_t eight_bits[8];
    uint32_t pos = (uint32_t)pos_i;
    switch (lcd->no_lines) {
        case 2:
            pos = 64*line + pos + 0b10000000;
            break;
        case 4:
            if (line == 0 || line == 2) {
                pos = 64*(line/2) + pos + 0b10000000;
            } else {
                pos = 64*((line-1)/2) + 20 + pos + 0b10000000;
            }
            break;
        default:
            pos = pos;
    }
    uint_into_8bits(eight_bits, pos);
    send_full_byte(lcd, COMMAND, eight_bits);
}

void LCDdisplay_print(LCDdisplay* lcd, const char* str) {
    uint32_t eight_bits[8];
    int i = 0;
    while (str[i] != 0) {
        uint_into_8bits(eight_bits, (uint32_t)(str[i]));
        send_full_byte(lcd, DATA, eight_bits);
        ++i;
    }
}

void LCDdisplay_print_wrapped(LCDdisplay* lcd, const char* str) {
    uint32_t eight_bits[8];
    int i = 0;

    LCDdisplay_goto_pos(lcd, 0, 0);

    while (str[i] != 0) {
        uint_into_8bits(eight_bits, (uint32_t)(str[i]));
        send_full_byte(lcd, DATA, eight_bits);
        ++i;
        if (i % lcd->no_chars == 0) {
            LCDdisplay_goto_pos(lcd, 0, i / lcd->no_chars);
        }
    }
}


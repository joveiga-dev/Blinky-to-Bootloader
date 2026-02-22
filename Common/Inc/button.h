#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>
#include <stdbool.h>
#include "driver_gpio.h"

/* STM32 User Button Definition */
#define USER_BUTTON1_BANK        GPIO_BANK_C
#define USER_BUTTON1_PIN         13

typedef enum {
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED
} button_state_t;

/* User Button api */
void user_button_init(void);
button_state_t user_button_read(void);
//void user_button_on(uint8_t led_num);
//void user_led_off(uint8_t led_num);
//void user_led_toggle(uint8_t led_num);
//void user_led_set(uint8_t led_num, gpio_pin_state_t state);

#endif /* __BUTTON_H__*/
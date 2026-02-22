#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>
#include <stdbool.h>
#include "driver_gpio.h"

/* STM32 User LED Definition */
#define USER_LED2_BANK        GPIO_BANK_A
#define USER_LED2_PIN         5

typedef enum {
    LED_OFF = 0,
    LED_ON
} led_state_t;

/* User LED api */
void user_led_init(void);
void user_led_on(void);
void user_led_off(void);
void user_led_toggle(void);
void user_led_set(gpio_pin_state_t state);

#endif /* __LED_H__*/
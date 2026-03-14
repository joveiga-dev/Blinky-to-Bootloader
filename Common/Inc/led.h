#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>
#include <stdbool.h>
#include "driver_gpio.h"

/* STM32 User LED Definition */
#define USER_LED2_BANK        GPIO_BANK_A
#define USER_LED2_PIN         5

/* External LEDS */
#define EXT_LED_BANK        GPIO_BANK_B
#define EXT_LED_PIN         5

typedef enum {
    LED_NUM_1 = 0,
    LED_NUM_2,
    LED_MAX_COUNT
} led_id_t;

typedef enum {
    LED_OFF = 0,
    LED_ON
} led_state_t;

typedef struct
{
    void (*set)(uint8_t val);
    void (*toggle)(void);
} led_t;


/* User LED api */
void user_led_init(void);

void user_led_on(led_id_t led);
void user_led_off(led_id_t led);
void user_led_toggle(led_id_t led);
void user_led_write(led_id_t led, gpio_pin_state_t state);

#endif /* __LED_H__*/
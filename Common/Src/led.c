#include "driver_gpio.h"
#include "led.h"

#include <stdlib.h>


typedef struct
{
    gpio_bank_t bank;
    uint16_t pin;
} led_hw_t;


// HW MAPPING TABLE FOR LED
static const led_hw_t led_hw[LED_MAX_COUNT] = {
    [LED_NUM_1] = {
        .bank = USER_LED2_BANK,
        .pin = USER_LED2_PIN
    },

    [LED_NUM_2] = {
        .bank = EXT_LED_BANK,
        .pin = EXT_LED_PIN
    }
};

static inline const led_hw_t *get_led(led_id_t led) {
    if (led >= LED_MAX_COUNT) {
        return NULL;
    }
    else {
        return &led_hw[led];
    }
}

/**
 * Initialize user LED on STM32
 */
void user_led_init(void)
{   
    for (uint32_t i = 0; i < LED_MAX_COUNT; i++)
    {
        gpio_enable_bank(led_hw[i].bank);
        gpio_mode_set(led_hw[i].bank, led_hw[i].pin, GPIO_MODE_OUTPUT);
        gpio_pin_clear(led_hw[i].bank, led_hw[i].pin);

    }
    
 
}

/**
 * Turn LED on
 */
void user_led_on(led_id_t led)
{   

   const led_hw_t *hw = get_led(led);
   if(!hw) {
        return;
   }

   gpio_pin_set(hw->bank, hw->pin);
    
}

/**
 * Turn LED off
 */
void user_led_off(led_id_t led)
{
   const led_hw_t *hw = get_led(led);
   if(!hw) {
        return;
   }

   gpio_pin_clear(hw->bank, hw->pin);
     
}
/**
 * Toggle LED
 */
void user_led_toggle(led_id_t led)
{
   const led_hw_t *hw = get_led(led);
   if(!hw) {
        return;
   }

   gpio_pin_toggle(hw->bank, hw->pin);
    
}

/**
 * Set LED to specific state
 */
void user_led_write(led_id_t led, gpio_pin_state_t state)
{
    
    const led_hw_t *hw = get_led(led);
    if(!hw) {
        return;
    }
    
    gpio_pin_write(hw->bank, hw->pin, state);
}

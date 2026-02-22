#include "driver_gpio.h"
#include "led.h"

/**
 * Initialize user LED on STM32
 */
void user_led_init(void)
{   
    gpio_enable_bank(GPIO_BANK_A); // Enable GPIO bank for LED
    gpio_mode_set(USER_LED2_BANK, USER_LED2_PIN, GPIO_MODE_OUTPUT); // Set pin mode to output

    // Turn the LED off initially
    gpio_pin_clear(USER_LED2_BANK, USER_LED2_PIN);
}

/**
 * Turn LED on
 */
void user_led_on(void)
{   
    gpio_pin_set(USER_LED2_BANK, USER_LED2_PIN);
    /*
    Se tiver mais de uma led na board
    switch (led_num)
    {
    case 2:
        gpio_pin_set(USER_LED2_BANK, USER_LED2_PIN);
        break;
    default:
        break;
    }
    */
    
}

/**
 * Turn LED off
 */
void user_led_off(void)
{
    gpio_pin_clear(USER_LED2_BANK, USER_LED2_PIN);
}
/**
 * Toggle LED
 */
void user_led_toggle(void)
{
    gpio_pin_toggle(USER_LED2_BANK, USER_LED2_PIN);
}

/**
 * Set LED to specific state
 */
void user_led_set(gpio_pin_state_t state)
{
    gpio_pin_write(USER_LED2_BANK, USER_LED2_PIN, state);
}

#include "driver_gpio.h"
#include "button.h"

/**
 * Initialize User Button
 */
void user_button_init(void)
{
    gpio_enable_bank(USER_BUTTON1_BANK); // Enable GPIO bank for Button
    gpio_mode_set(USER_BUTTON1_BANK, USER_BUTTON1_PIN, GPIO_MODE_INPUT); // Set pin mode to input
    gpio_pull_set(USER_BUTTON1_BANK, USER_BUTTON1_PIN, GPIO_PULL_UP); // define pull up

}

/**
 * Read button state
 */
button_state_t user_button_read(void)
{
    gpio_pin_state_t state = gpio_pin_read(USER_BUTTON1_BANK, USER_BUTTON1_PIN);

    if ( state == GPIO_PIN_LOW)
    {
        return BUTTON_PRESSED;
    }
    else if(state == GPIO_PIN_HIGH)
    {
        return BUTTON_RELEASED;
    }
    else
    {
        return BUTTON_UNKNOWN;
    }
}
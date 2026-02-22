#include "driver_gpio.h"
#include "button.h"

/**
 * Initialize User Button
 */
void user_button_init(void)
{
    gpio_enable_bank(GPIO_BANK_C); // Enable GPIO bank for Button
    gpio_mode_set(USER_BUTTON1_BANK, USER_BUTTON1_PIN, GPIO_MODE_INPUT); // Set pin mode to input
    gpio_pull_set(USER_BUTTON1_BANK, USER_BUTTON1_PIN, GPIO_PULL_UP); // define pull up

}

/**
 * Read button state
 */
button_state_t user_button_read(void)
{
    return (gpio_pin_read(USER_BUTTON1_BANK, USER_BUTTON1_PIN) == GPIO_PIN_LOW) ? BUTTON_PRESSED : BUTTON_RELEASED;
}
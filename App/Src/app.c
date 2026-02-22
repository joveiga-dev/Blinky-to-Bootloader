#include <stdint.h>
#include "driver_gpio.h"
#include "led.h"
#include "button.h"
#include "app.h"


#define FAST_DELAY      (100000)
#define SLOW_DELAY      (500000)

void delay(volatile uint32_t count) {
    while (count--);  
}

 void app_init(void)
 {
    user_led_init();
    user_button_init();
 }

 void app_task(void)
 {
    button_state_t btn_state = user_button_read();
        if(BUTTON_PRESSED == btn_state)
        {
            user_led_toggle();
            delay(FAST_DELAY);
        }
        else
        {
            user_led_off();
        }
 }
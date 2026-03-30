#include <stdint.h>
#include "Led.h"
#include "SystickTimer.h"
#include "Button.h"
#include "App.h"
#include <stdio.h>

#define BLINK_INTERVAL_MS   200
#define UART_INTERVAL_MS    500

//static uint32_t button_press_counter = 0;

//BTN_State_t last_btn_state = BTN_RELEASED;


void App_Init(void)
{
    Led_InitAllLeds();
    Btn_Init(BTN1);

    systick_timer_init(TICKS_FOR_1MS);
    
}

void App_Task(void)
{
    static uint32_t last_time = 0;
    static Btn_State_t last_btn_state = BTN_RELEASED;

    uint32_t current_time = systicktimer_get_current_count();
    Btn_State_t btn_state = Btn_Read(BTN1);

    if ((current_time - last_time) >= BLINK_INTERVAL_MS)
    {
        last_time = current_time;

        if (btn_state != BTN_PRESSED) {
            Led_Toggle(LED1);
        }
        
    }

    if (btn_state == BTN_PRESSED && last_btn_state == BTN_RELEASED){
        Led_On(LED2);
    }

    else if (btn_state == BTN_RELEASED)
    {
        Led_Off(LED2);
    }

}


/*
 void app_init(void)
 {
    user_led_init();
    user_button_init();
    systick_timer_init(TICKS_FOR_1MS);

    uart_config(USART2, 9600);

    uart_write_string("Button Counter Project Started!\r\n");

 }

 void app_task(void)
 {
    static uint32_t last_time = 0;
    static uint32_t last_uart = 0;
    button_state_t btn_state = user_button_read();
    char buffer[50];

    uint32_t current_time = systicktimer_get_current_count();

    if ((current_time - last_time) >= BLINK_INTERVAL_MS) {
        last_time = current_time;

        if (btn_state != BUTTON_PRESSED) {
            user_led_toggle(LED_NUM_1);
        }
    }

    if (btn_state == BUTTON_PRESSED && last_btn_state == BUTTON_RELEASED){
        button_press_counter++;
        user_led_on(LED_NUM_2);
        //sprintf(buffer, "Counter: %lu\r\n", button_press_counter);
        //uart_write_string(buffer);

    }
    else if (btn_state == BUTTON_RELEASED)
    {
        user_led_off(LED_NUM_2);
    }

    last_btn_state = btn_state;

    if ((current_time - last_uart) >= UART_INTERVAL_MS)
    {
        last_uart = current_time;
        sprintf(buffer, "Button pressed: %lu times\r\n", button_press_counter);
        uart_write_string(buffer);
    }
    
}
*/ 
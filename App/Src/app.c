#include <stdint.h>
#include <stdio.h>

#include "Led.h"
#include "SysTick.h"
#include "Button.h"
#include "App.h"
#include "Led_fsm.h"
#include "USerial.h"

#define UART_INTERVAL_MS    500

void Usart_Init(void)
{
    static USART_Handle huart2;
    
    huart2.Usartx.Port = USART2;
    huart2.Usartx.baud_rate = 9600;
    huart2.Usartx.data_bits = 8;
    huart2.Usartx.stop_bits = 1;
    huart2.Usartx.parity = 0;
    huart2.Usartx.oversampling = 0;

    huart2.Gpiox.TX_Port = GPIOA;
    huart2.Gpiox.tx_pin = 2;

    huart2.Gpiox.RX_Port = GPIOA;
    huart2.Gpiox.rx_pin = 3;

    huart2.Gpiox.af = 7;

    Userial_Init(&huart2);

    Userial_WriteString(&huart2, "UART READY\r\n");
}

void App_Init(void)
{
    Led_InitAllLeds();
    Btn_Init(BTN1);
    SysTick_Init(SYSTICK_LOAD_1MS);
    LedFsm_Init();
    Usart_Init();
}

static void Led_Task(void)
{
    uint32_t current_time = SysTick_GetTimeMs(); 
    LedFsm_Update(current_time);
}

static void Button_Task(void)
{
    static Btn_State_t last_btn_state = BTN_RELEASED;
    Btn_State_t btn_state = Btn_Read(BTN1);
    static uint32_t btn_press_counter = 0;

    if (btn_state == BTN_PRESSED && last_btn_state == BTN_RELEASED)
    {
        LedFsm_EventHandle(USER_LED_EVENT_BUTTON_PRESSED);
        btn_press_counter++;
    }
    else if (btn_state == BTN_RELEASED)
    {
        LedFsm_EventHandle(USER_LED_EVENT_BUTTON_RELEASED);
    }

    last_btn_state = btn_state;
}

void App_Task(void)
{
    Led_Task();
    Button_Task();
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
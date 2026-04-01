#include <stdint.h>
#include <stdio.h>

#include "Led.h"
#include "SysTick.h"
#include "Button.h"
#include "App.h"
#include "Led_fsm.h"
#include "USerial.h"

#define UART_INTERVAL_MS    500

USART_Handle huart2;

void Usart2_Init(void)
{
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

    Userial_WriteString(&huart2, "USART READY\r\n");
}

void App_Init(void)
{
    Led_InitAllLeds();
    Btn_Init(BTN1);
    SysTick_Init(SYSTICK_LOAD_1MS);
    LedFsm_Init();
    Usart2_Init();

    Userial_WriteString(&huart2, "App INIT\r\n");
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

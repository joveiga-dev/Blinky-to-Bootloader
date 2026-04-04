#include <stdint.h>
#include <stdio.h>

#include "Led.h"
#include "SysTick.h"
#include "Button.h"
#include "App.h"
#include "Led_fsm.h"
#include "USerial.h"

#define BOOTLOADER_SIZE    (0x8000U)


static USART_Handle huart2;

static void Vector_Setup(void)
{
    SCB_VTOR = BOOTLOADER_SIZE;
}

/**
 * 
 */
void Usart2_Init(void)
{
    huart2.Usartx.Port = USART2;
    huart2.Usartx.baud_rate = BR_USART2;
    huart2.Usartx.DataBits = 8;
    huart2.Usartx.StopBits = 1;
    huart2.Usartx.Parity = 0;
    huart2.Usartx.Mode = MODE_TX_RX;
    huart2.Usartx.HwFlowCtl = HW_CONTROL_NONE;
    huart2.Usartx.OverSampling = 0;

    huart2.Gpiox.TX_Port = GPIOA;
    huart2.Gpiox.tx_pin = TX_PIN_USART2;

    huart2.Gpiox.RX_Port = GPIOA;
    huart2.Gpiox.rx_pin = RX_PIN_USART2;

    huart2.Gpiox.af = AF_USART2;

    Userial_Init(&huart2);

    Userial_SendString(&huart2,  "USART Ready\r\n");
}

/**
 * 
 */
void App_Init(void)
{
    Vector_Setup();
    Led_InitAllLeds();
    Btn_Init(BTN1);
    SysTick_Init(SYSTICK_LOAD_1MS);
    LedFsm_Init();
    Usart2_Init();

    Userial_SendString(&huart2,  "App INIT\r\n");
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

/*
static void USart_PollingTask(void)
{
    Userial_PollReceive(&huart2);

    if (Userial_Data_Available())
    {
        // Read the data received
        uint8_t rx = Userial_ReceiveByte();
        Userial_SendByte(&huart2, rx +1);
    }
}
*/

static void USart_InterruptTask(void)
{
    if (Userial_Data_Available())
    {
        // Read the data received
        uint8_t rx = Userial_ReceiveByte();
        Userial_SendByte(&huart2, rx +1);
    }

}



void App_Task(void)
{
    Led_Task();
    Button_Task();
    USart_InterruptTask();
}

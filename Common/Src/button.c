#include "Gpio.h"
#include "Button.h"

// HW MAPPING TABLE FOR Button
static const Btn_t Btn[BTN_COUNT] = {
    [BTN1] = {  // User Btn
        .Gpiox = USER_BUTTON1_BANK,
        .pin = USER_BUTTON1_PIN,
        .active_state = BTN_ACTIVE_LOW
    }
    // Add other buttons in the future (Extern)

};

/**
 * Initialize User Button
*/

void Btn_Init(Btn_Id_t Btnx)
{
    GPIO_Pin_Config_t config = {
        .Port = Btn[Btnx].Gpiox,
        .pin = Btn[Btnx].pin,
        .Mode = GPIO_MODE_INPUT,
        .Otype = GPIO_OTYPE_PUSH_PULL,
        .Pull = GPIO_PULL_UP,
        .Speed = GPIO_LOW_SPEED,
    };

    GPIO_Init(&config);
}


/*
* Read button state
*
*/
Btn_State_t Btn_Read(Btn_Id_t Btnx)
{
    const Btn_t *btn = &Btn[Btnx];
    GPIO_Pin_State_t state = GPIO_ReadPin(btn->Gpiox, btn->pin);
    if(btn->active_state == BTN_ACTIVE_LOW && state == GPIO_PIN_LOW)
    {
        return BTN_PRESSED;
    }
    else
    {
        return BTN_RELEASED;
    }
    
}





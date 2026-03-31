#include "Gpio.h"
#include "Led.h"


// HW MAPPING TABLE FOR LED
static const Led_t Led[LEDN] = {
    [LED1] = {  // User Led -> PA5
        .Gpiox = USER_LED2_BANK,
        .pin = USER_LED2_PIN,
        .active_state = LED_ACTIVE_HIGH
    },

    [LED2] = {  // External Led -> PB5
        .Gpiox = EXT_LED_BANK,
        .pin = EXT_LED_PIN,
        .active_state = LED_ACTIVE_HIGH
    }

};

/**
 * 
 */
void Led_Init(Led_Id_t Ledx)
{

    if (Ledx >= LEDN)
    {
        return;
    }

    GPIO_Pin_Config_t config = {
        .Port = Led[Ledx].Gpiox,
        .pin = Led[Ledx].pin,
        .Mode = GPIO_MODE_OUTPUT,
        .Otype = GPIO_OTYPE_PUSH_PULL,
        .Pull = GPIO_PULL_NONE,
        .Speed = GPIO_LOW_SPEED,
    };

    GPIO_Init(&config);
    Led_Off(Ledx);

}

void Led_InitAllLeds(void)
{
    for (Led_Id_t i = 0; i < LEDN; i++)
    {
        Led_Init(i);
    }
    
}

void Led_DeInitAllLeds(void)
{
    for (Led_Id_t i = 0; i < LEDN; i++)
    {
        Led_Off(i);
    }
}

// Helper
static void Led_Write(const Led_t *Ledx, Led_State_t State)
{
    if (Ledx->active_state == LED_ACTIVE_HIGH)
    {
        GPIO_WritePin(Ledx->Gpiox, Ledx->pin, State ? GPIO_PIN_HIGH : GPIO_PIN_LOW);
    }
    else
    {
        GPIO_WritePin(Ledx->Gpiox, Ledx->pin,  State ? GPIO_PIN_LOW : GPIO_PIN_HIGH);
    }
}


void Led_On(Led_Id_t Ledx)
{
    if (Ledx >= LEDN) return;
    
    Led_Write(&Led[Ledx], LED_STATE_ON);
}

void Led_Off(Led_Id_t Ledx)
{
    if (Ledx >= LEDN) return;
    
    Led_Write(&Led[Ledx], LED_STATE_OFF);
}

void Led_Toggle(Led_Id_t Ledx)
{
    if (Ledx >= LEDN)
        return;

    GPIO_TogglePin(Led[Ledx].Gpiox, Led[Ledx].pin);
}






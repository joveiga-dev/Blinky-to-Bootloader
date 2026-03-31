#include "Led_fsm.h"
#include "Led.h"
#include "SysTick.h"

static eLedState current_state = USER_LED_STATE_OFF;
static uint32_t last_toggle = 0;

void LedFsm_Init(void)
{
    current_state = USER_LED_STATE_OFF;
    last_toggle = 0;
}

void LedFsm_EventHandle(eLedEvent event)
{
    if (event == USER_LED_EVENT_BUTTON_PRESSED)
    {
        LedFsm_NextState();
        Led_On(LED2);
    }
    else if (event == USER_LED_EVENT_BUTTON_RELEASED)
    {
        Led_Off(LED2);
    }
    
}

void LedFsm_Update(uint32_t time_ms)
{
    switch (current_state)
    {
    case USER_LED_STATE_OFF:
        Led_Off(LED1);
        break;

    case USER_LED_STATE_ON:
        Led_On(LED1);
        break;

    case USER_LED_STATE_BLINK_SLOW:
        if ((time_ms - last_toggle) >= USER_LED_SLOW_BLINK)
        {
            last_toggle = time_ms;
            Led_Toggle(LED1);
        }
        break;

    case USER_LED_STATE_BLINK_FAST:
        if ((time_ms - last_toggle) >= USER_LED_FAST_BLINK)
        {
            last_toggle = time_ms;
            Led_Toggle(LED1);
        }
        break;
    
    default:
        current_state =  USER_LED_STATE_OFF;
        break;
    }
}

void LedFsm_NextState(void)
{
    current_state =  (eLedState)(current_state + 1);

    if (current_state >= USER_LED_STATE_MAX)
    {
        current_state = USER_LED_STATE_OFF;
    }

    last_toggle = 0;
}
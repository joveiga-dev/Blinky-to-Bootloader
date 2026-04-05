/**
 * Led_fsm.h
 * 
 * @date: Marc, 2026
 * @author: JV
 * 
 */

#ifndef LED_FSM_H_
#define LED_FSM_H_

#include "Common_Defines.h"


#define USER_LED_SLOW_BLINK   500
#define USER_LED_FAST_BLINK   100 // MS

typedef enum {
    USER_LED_STATE_OFF = 0,
    USER_LED_STATE_BLINK_SLOW,
    USER_LED_STATE_BLINK_FAST,
    USER_LED_STATE_ON,

    USER_LED_STATE_MAX
} eLedState;

typedef enum {
    USER_LED_EVENT_NONE = 0,
    USER_LED_EVENT_BUTTON_PRESSED,
    USER_LED_EVENT_BUTTON_RELEASED,

    USER_LED_MAX_EVENTS
} eLedEvent;


// Function Prototypes
void LedFsm_Init(void);
void LedFsm_Update(uint32_t time_ms);
void LedFsm_EventHandle(eLedEvent event);
void LedFsm_NextState(void);
#endif
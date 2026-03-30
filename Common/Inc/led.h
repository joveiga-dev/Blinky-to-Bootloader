#ifndef LED_H_
#define LED_H_

#include "Gpio.h"

/* STM32 User LED Definition */
#define USER_LED2_BANK        GPIOA
#define USER_LED2_PIN         GPIO_PIN_5

/* External LEDS */
#define EXT_LED_BANK          GPIOB
#define EXT_LED_PIN           GPIO_PIN_5

typedef enum {
    LED1 = 0,
    LED2,
    LEDN
} Led_Id_t;

typedef enum {
    LED_ACTIVE_HIGH = 0,
    LED_ACTIVE_LOW
} Led_Active_t;

typedef enum {
    LED_STATE_OFF = 0,
    LED_STATE_ON,
    LED_STATE_3BLINKS_OFF,
    LED_STATE_FAST_BLINK,
    LED_STATE_SLOW_BLINK
} Led_State_t;

// LED Control Application
typedef struct {
    GPIO_RegDef_t * Gpiox;
    uint8_t pin;
    Led_Active_t active_state;
}Led_t;


/* User LED api */
void Led_Init(Led_Id_t Ledx);
void Led_InitAllLeds(void);
void Led_DeInitAllLeds(void);
void Led_Toggle(Led_Id_t Ledx);
void Led_On(Led_Id_t Ledx);
void Led_Off(Led_Id_t Ledx);


#endif /* LED_H_*/
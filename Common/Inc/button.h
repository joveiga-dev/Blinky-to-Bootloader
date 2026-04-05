#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Gpio.h"

#include "Common_Defines.h"

/* STM32 User Button Definition */
#define USER_BUTTON1_BANK        GPIOC
#define USER_BUTTON1_PIN         GPIO_PIN_13

typedef enum {
    BTN1 = 0,
    BTN2,
    BTN_COUNT
} Btn_Id_t;

typedef enum {
    BTN_ACTIVE_HIGH = 0,
    BTN_ACTIVE_LOW
} Btn_Active_t;

typedef enum {
    BTN_RELEASED = 0,
    BTN_PRESSED
} Btn_State_t;

// Button interface application
typedef struct {
    GPIO_RegDef_t * Gpiox;
    uint8_t pin;
    Btn_Active_t active_state;
    bool last_state;
    bool current_state;
} Btn_t;


/* User Button api */
void Btn_Init(Btn_Id_t Btnx);
Btn_State_t Btn_Read(Btn_Id_t Btnx);
bool Btn_read(Btn_Id_t Btnx);
bool Btn_Pressed(Btn_Id_t Btnx);

#endif /* BUTTON_H_*/
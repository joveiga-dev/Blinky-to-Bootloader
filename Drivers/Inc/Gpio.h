/**
 * STM32Lx GPIO Driver (General Purpose Input Output)
 * 
 * The STM32Lx has 8 GPIO banks (GPIOA-GPIOH), each with 16 pins
 * Total: 
 *  
 */

 #ifndef __GPIO_H__
 #define __GPIO_H__

 #include <stdint.h>
 #include <stdbool.h>
 #include "Stm32l4xx.h"


typedef enum {
    GPIO_BANK_A = 0,
    GPIO_BANK_B,
    GPIO_BANK_C,
    GPIO_BANK_D,
    GPIO_BANK_E,
    GPIO_BANK_F,
    GPIO_BANK_G,
    GPIO_BANK_H,
    GPIO_BANK_COUNT
} GPIO_Bank_t;

typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALTERNATE,
    GPIO_MODE_ANALOG,
    GPIO_MODE_UNKNOWN
} GPIO_Mode_t;

typedef enum {
    GPIO_OUTPUT_PUSH_PULL,
    GPIO_OUTPUT_OPEN_DRAIN
} GPIO_Output_Type_t;

typedef enum {
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN,
    GPIO_RESERVED
} GPIO_Pull_t;

typedef enum {
    GPIO_PIN_LOW =0,
    GPIO_PIN_HIGH
} GPIO_Pin_State_t;

typedef enum {
    GPIO_LOW_SPEED = 0,
    GPIO_MEDIUM_SPEED,
    GPIO_HIGH_SPEED,
    GPIO_VERY_HIGH_SPEED
} GPIO_Pin_Output_Speed_t;

typedef struct 
{
    GPIO_RegDef_t *Port;
    uint8_t pin;
    GPIO_Mode_t mode;
    GPIO_Output_Type_t otype;
    GPIO_Pull_t pull;
    GPIO_Pin_Output_Speed_t speed;
    uint8_t alternate_func;
} GPIO_Pin_Config_t;

/* Helper macros */
#define GPIO_PIN_MASK(pin)          (1U << (pin))


/* Function Prototypes*/
void GPIO_Clock_Enable(GPIO_RegDef_t *Port);
void GPIO_Clock_Disable(GPIO_RegDef_t *Port);

void GPIO_Init(GPIO_Pin_Config_t *Gpiox);

/* Pin Operations */
void GPIO_WritePin(GPIO_RegDef_t *Port, uint8_t pin, GPIO_Pin_State_t state);
void GPIO_SetPin(GPIO_RegDef_t *Port, uint8_t pin); // Wrapper
void GPIO_ResetPin(GPIO_RegDef_t *Port, uint8_t pin); // Wrapper
GPIO_Pin_State_t GPIO_ReadPin(GPIO_RegDef_t *Port, uint8_t pin);
void GPIO_TogglePin(GPIO_RegDef_t *Port, uint8_t pin);

/* Config */
void GPIO_SetPinMode(GPIO_RegDef_t *Port, uint8_t pin, GPIO_Mode_t mode);
GPIO_Mode_t GPIO_GetPinMode(GPIO_RegDef_t *Port, uint8_t pin);
void GPIO_SetPinPull(GPIO_RegDef_t *Port, uint8_t pin, GPIO_Pull_t pull);
void GPIO_SetPinAlternateFunction(GPIO_RegDef_t *Port, uint8_t pin, uint8_t af);
void GPIO_SetPinOutputSpeed(GPIO_RegDef_t *Port, uint8_t pin, GPIO_Pin_Output_Speed_t speed);
void GPIO_SetPinOutputType(GPIO_RegDef_t *Port, uint8_t pin, GPIO_Output_Type_t otype);

/* Port Operations */
void GPIO_WritePort(GPIO_RegDef_t *Port, uint16_t value);
void GPIO_SetPort(GPIO_RegDef_t *Port, uint16_t mask);
uint16_t GPIO_ReadPort(GPIO_RegDef_t *Port);
void GPIO_ResetPort(GPIO_RegDef_t *Port, uint16_t mask);


#endif /* GPIO_H */
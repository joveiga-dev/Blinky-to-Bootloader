/**
 * STM32Lx GPIO Driver (General Purpose Input Output)
 * 
 * The STM32Lx has 8 GPIO banks (GPIOA-GPIOH), each with 16 pins
 * Total: 
 *  
 */

 #ifndef GPIO_H_
 #define GPIO_H_

 #include <stdint.h>
 #include "Stm32l4xx.h"

 typedef enum {
    GPIO_PIN_0 = 0x0000,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15
} GPIO_Pin_t;


typedef enum {
    GPIO_MODE_INPUT = 0x00U,
    GPIO_MODE_OUTPUT = 0x01U,
    GPIO_MODE_ALT = 0x02U,
    GPIO_MODE_ANALOG = 0x03U
} GPIO_Mode_t;

typedef enum {
    GPIO_OTYPE_PUSH_PULL = 0x00U,
    GPIO_OTYPE_OPEN_DRAIN = 0x01U
} GPIO_Output_Type_t;

typedef enum {
    GPIO_PULL_NONE = 0x00U,
    GPIO_PULL_UP = 0x01U,
    GPIO_PULL_DOWN = 0x02U,
} GPIO_Pull_t;

/**
 * @brief GPio Pin State
 */
typedef enum {
    GPIO_PIN_LOW = 0x00U,
    GPIO_PIN_HIGH = 0x01U
} GPIO_Pin_State_t;

typedef enum {
    GPIO_LOW_SPEED = 0x00U,
    GPIO_MEDIUM_SPEED = 0x01U,
    GPIO_HIGH_SPEED = 0x02U,
    GPIO_VERY_HIGH_SPEED = 0x03U
} GPIO_Pin_Output_Speed_t;

typedef struct 
{
    GPIO_RegDef_t *Port;
    uint8_t pin;

    GPIO_Mode_t Mode;
    GPIO_Output_Type_t Otype;
    GPIO_Pull_t Pull;
    GPIO_Pin_Output_Speed_t Speed;

    uint8_t Alternate;
} GPIO_Pin_Config_t;

/* Helper macros */
#define GPIO_PIN_MASK(pin)          (1U << (pin))


/* Function Prototypes*/
void GPIO_Clock_Enable(GPIO_RegDef_t *Gpiox); //
void GPIO_Clock_Disable(GPIO_RegDef_t *Gpiox); //

int GPIO_Init(const GPIO_Pin_Config_t *Gpiox);
int GPIO_Deinit(const GPIO_Pin_Config_t *Gpiox);

void GPIO_SetPin(GPIO_RegDef_t *Gpiox, uint8_t pin);
void GPIO_ResetPin(GPIO_RegDef_t *Gpiox, uint8_t pin);

void GPIO_WritePin(GPIO_RegDef_t *Gpiox, uint8_t pin, GPIO_Pin_State_t State); //
GPIO_Pin_State_t GPIO_ReadPin(GPIO_RegDef_t *Gpiox, uint8_t pin); //
void GPIO_TogglePin(GPIO_RegDef_t *Gpiox, uint8_t pin); //

/* Config */
int GPIO_SetPinMode(GPIO_RegDef_t *Gpiox, uint8_t pin, GPIO_Mode_t Mode);
int GPIO_SetPinPull(GPIO_RegDef_t *Gpiox, uint8_t pin, GPIO_Pull_t Pull);
int GPIO_SetPinAF(GPIO_RegDef_t *Gpiox, uint8_t pin, uint8_t af);
int GPIO_SetPinOutputSpeed(GPIO_RegDef_t *Gpiox, uint8_t pin, GPIO_Pin_Output_Speed_t Speed);
int GPIO_SetPinOutputType(GPIO_RegDef_t *Gpiox, uint8_t pin, GPIO_Output_Type_t Otype);

/* Port Operations */
void GPIO_WritePort(GPIO_RegDef_t *Gpiox, uint16_t value);
void GPIO_SetPort(GPIO_RegDef_t *Gpiox, uint16_t mask);
uint16_t GPIO_ReadPort(GPIO_RegDef_t *Gpiox);
void GPIO_ResetPort(GPIO_RegDef_t *Gpiox, uint16_t mask);



#endif /* GPIO_H_*/
#include "Gpio.h"
#include "Stm32l4xx.h"

#include <stdio.h>

/**
 *  GPIO_Clock_Enable
 * @brief Enable Clock for a given GPIO Port
 * @return None
 */
void GPIO_Clock_Enable(GPIO_RegDef_t *Gpiox)
{
    uint32_t index_port = GET_GPIO_INDEX(Gpiox);
    if (index_port < 8U)
    {
        RCC->RCC_AHB2ENR |= GPIO_PIN_MASK(index_port);
        (void)RCC->RCC_AHB2ENR;
    }
}

/**
 *  GPIO_Clock_Disable
 * @brief Disable Clock for a given GPIO Port
 * @return None
 */
void GPIO_Clock_Disable(GPIO_RegDef_t *Gpiox)
{
    uint32_t index_port =  GET_GPIO_INDEX(Gpiox);

    if (index_port < 8U)
    {
        RCC->RCC_AHB2ENR &= ~GPIO_PIN_MASK(index_port);
        // Wait for the module to become operational 
        (void)RCC->RCC_AHB2ENR; // dummy
    }
}


void GPIO_SetPin(GPIO_RegDef_t *Gpiox, uint8_t pin)
{
    Gpiox->BSRR = (1U << pin);
}
void GPIO_ResetPin(GPIO_RegDef_t *Gpiox, uint8_t pin)
{
    Gpiox->BSRR = (1U << (pin + 16U));
}

/**
 * GPIO_TogglePin
 * @brief Toggle pin for a specific port
 */
void GPIO_TogglePin(GPIO_RegDef_t *Gpiox, uint8_t pin)
{

    if (!Gpiox || pin > 15U)
    {
        return;
    }

    uint32_t curr_state = Gpiox->ODR;

    // Read current state and toggle
    if (curr_state & GPIO_PIN_MASK(pin))
    {
        GPIO_ResetPin(Gpiox, pin);
    }
    else
    {
        GPIO_SetPin(Gpiox, pin);
    }
}

/**
 * 
 * @param Port: GPIO (a, B, C, ...)
 * @param pin: Pin (0-15)
 * @param mode: Mode (Input, output, Alternate Function, Analog)
 */
int GPIO_SetPinMode(GPIO_RegDef_t *Gpiox, uint8_t pin, GPIO_Mode_t Mode)
{   
    if (!Gpiox || pin > 15U)
    {
        return -1;
    }

    uint32_t shift = pin *2U;

    Gpiox->MODER &= ~(3U << shift);
    Gpiox->MODER |= ((uint32_t)  Mode & 3U) << shift;
    return 0;
}


/**
 * GPIO_WritePin
 */
void GPIO_WritePin(GPIO_RegDef_t *Gpiox, uint8_t pin, GPIO_Pin_State_t State)
{
    if (!Gpiox || pin > 15U)
    {
        return;
    }

    if (GPIO_PIN_HIGH == State)
    {
        GPIO_SetPin(Gpiox, pin);
    }
    else
    {
        GPIO_ResetPin(Gpiox, pin);
    }
}

/**
 * GPIO_ReadPin
 */

GPIO_Pin_State_t GPIO_ReadPin(GPIO_RegDef_t *Gpiox, uint8_t pin)
{
    if (!Gpiox || pin > 15U)
    {
        return GPIO_PIN_LOW;
    }

    return ((Gpiox->IDR & GPIO_PIN_MASK(pin)) != 0U)
            ? GPIO_PIN_HIGH
            : GPIO_PIN_LOW;
}

/**
 * Configure Pull-up down
 */
int GPIO_SetPinPull(GPIO_RegDef_t *Gpiox, uint8_t pin, GPIO_Pull_t Pull)
{
    if (!Gpiox || pin > 15U)
    {
        return -1;
    }

    uint32_t shift = pin * 2U;

    Gpiox->PUPDR &= ~(3U << shift);
    Gpiox->PUPDR |= ((uint32_t) Pull & 3U) << shift;
    return 0;
}

/**
 * @param Port: GPIO (A, B, C, ...)
 * @param pin: Pin (0-15)
 * @param af: Value of function (0 a 15)
 */
int GPIO_SetPinAF(GPIO_RegDef_t *Gpiox, uint8_t pin, uint8_t af)
{
    
    if (!Gpiox || pin > 15U || af > 15U)
    {
        return -1;
    }

    uint8_t afr_index = pin >> 3U;
    uint32_t shift = (pin & 7U) * 4U;

    Gpiox->AFR[afr_index] &= ~(0xFU << shift);
    Gpiox->AFR[afr_index] |= ((uint32_t) af << shift);
    return 0;
}

/**
 * @details Configure the IO Output Speed
 * @param Port: GPIO (A, B, C, ...)
 * @param pin: Pin (0-15)
 * @param speed: enum (Low, Medium, High, Very High)
 */
int GPIO_SetPinOutputSpeed(GPIO_RegDef_t *Gpiox, uint8_t pin, GPIO_Pin_Output_Speed_t Speed)
{
    if (!Gpiox || pin > 15U)
    {
        return -1;
    }

    uint32_t shift = pin *2U;
    Gpiox->OSPEEDR &= ~(3U << shift);
    Gpiox->OSPEEDR |= ((uint32_t) Speed & 3U) << shift;
    return 0;
}

/**
 * Configure the I/O Output type.
 * @param Port: GPIO (A, B, C, ...)
 * @param pin: Pin (0-15)
 * @param otype: enum (Push-pull, Open-drain)
 */
int GPIO_SetPinOutputType(GPIO_RegDef_t *Gpiox, uint8_t pin, GPIO_Output_Type_t Otype)
{
    if (!Gpiox || pin > 15U)
    {
        return -1;
    }

    if (Otype == GPIO_OTYPE_OPEN_DRAIN)
    {
        Gpiox->OTYPER |= (1U << pin);
    }
    else
    {
        Gpiox->OTYPER &= ~(1U << pin);
    }
    return 0;

}

/**
 * 
 */
int GPIO_Init(const GPIO_Pin_Config_t *Gpiox)
{
    if (!Gpiox || !Gpiox->Port || Gpiox->pin > 15U)
    {
        return -1;
    }
    
    GPIO_Clock_Enable(Gpiox->Port);

    GPIO_SetPinMode(Gpiox->Port, Gpiox->pin, Gpiox->Mode);
    GPIO_SetPinPull(Gpiox->Port, Gpiox->pin, Gpiox->Pull);

    if (Gpiox->Mode == GPIO_MODE_OUTPUT)
    {
        GPIO_SetPinOutputType(Gpiox->Port, Gpiox->pin, Gpiox->Otype);
        GPIO_SetPinOutputSpeed(Gpiox->Port, Gpiox->pin, Gpiox->Speed);
    }

    if (Gpiox->Mode == GPIO_MODE_ALT)
    {
        GPIO_SetPinAF(Gpiox->Port, Gpiox->pin, Gpiox->Alternate);
    }

    return 0;
    
}

/**
 * Write to entire GPIO port (all 16 pins)
 */
void GPIO_WritePort(GPIO_RegDef_t *Gpiox, uint16_t value)
{
    if(!Gpiox) return;
    Gpiox->ODR = (uint32_t)value;
}

void GPIO_SetPort(GPIO_RegDef_t *Gpiox, uint16_t mask)
{   
    if(!Gpiox) return;
    Gpiox->BSRR = (uint32_t) mask;
}
/**
 * Read entire GPIO port (all 16 pins)
 */
uint16_t GPIO_ReadPort(GPIO_RegDef_t *Gpiox)
{
    if(!Gpiox) return 0;
    return (uint16_t)Gpiox->IDR;
}

void GPIO_ResetPort(GPIO_RegDef_t *Gpiox, uint16_t mask)
{
    if(!Gpiox) return;
    Gpiox->BSRR = (uint16_t) mask << 16U;
}






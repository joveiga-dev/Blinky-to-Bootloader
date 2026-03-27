#include "Gpio.h"
#include "Stm32l4xx.h"

/**
 *  GPIO_Clock_Enable
 * @brief Enable Clock for a given GPIO Port
 * @return None
 */
void GPIO_Clock_Enable(GPIO_RegDef_t *Port)
{
    uint32_t index_port = GET_GPIO_INDEX(Port);
    if (index_port < 8)
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
void GPIO_Clock_Disable(GPIO_RegDef_t *Port)
{
    uint32_t index_port =  GET_GPIO_INDEX(Port);
    if (index_port < 8)
    {
        RCC->RCC_AHB2ENR &= ~GPIO_PIN_MASK(index_port);
        // Wait for the module to become operational 
        (void)RCC->RCC_AHB2ENR; // dummy
    }
}

/**
 * GPIO_SetPin
 * Set pin for a specific port
 */
void GPIO_SetPin(GPIO_RegDef_t *Port, uint8_t pin)
{
    if (GET_GPIO_INDEX(Port) > 8 || pin > 15)
    {
        return;
    }

    //Port->ODR |= GPIO_PIN_MASK(pin);
    Port->BSRR = GPIO_PIN_MASK(pin);
}

/**
 * GPIO_ClearPin
 * @brief Clear pin for a specific port
 */
void GPIO_ResetPin(GPIO_RegDef_t *Port, uint8_t pin)
{   
    if (GET_GPIO_INDEX(Port) > 8 || pin > 15)
    {
        return;
    }

    //Port->ODR &= ~GPIO_PIN_MASK(pin);
    Port->BSRR |= GPIO_PIN_MASK(pin + 16);
}

/**
 * GPIO_TogglePin
 */
void GPIO_TogglePin(GPIO_RegDef_t *Port, uint8_t pin)
{
    if (GET_GPIO_INDEX(Port) > 8 || pin > 15)
    {
        return;
    }

    Port->ODR ^= GPIO_PIN_MASK(pin);
}

/**
 * 
 * @param Port: GPIO (a, B, C, ...)
 * @param pin: Pin (0-15)
 * @param mode: Mode (Input, output, Alternate Function, Analog)
 */
void GPIO_SetPinMode(GPIO_RegDef_t *Port, uint8_t pin, GPIO_Mode_t mode)
{
    if (GET_GPIO_INDEX(Port) > 8 || pin > 15)
    {
        return;
    }
    Port->MODER &= ~(3U << (pin * 2)); // clear previous mode
    Port->MODER |= ((uint32_t) mode & 3) << (pin *2 ); // set new mode
}

/**
 * GPIO_GetPinMode
 * @brief 
 */
GPIO_Mode_t GPIO_GetPinMode(GPIO_RegDef_t *Port, uint8_t pin)
{
    
    if (GET_GPIO_INDEX(Port) > 8 || pin > 15)
    {
        return;
    }

    return (GPIO_Mode_t)((Port->MODER >> (pin *2)) & 0x3); // (reg >> offset ) & mask
}

/**
 * GPIO_WritePin
 */
void GPIO_WritePin(GPIO_RegDef_t *Port, uint8_t pin, GPIO_Pin_State_t state)
{
    
    if (GPIO_PIN_HIGH == state)
    {
        GPIO_SetPin(Port, pin);
    }
    else
    {
        GPIO_ClearPin(Port, pin);
    }
}

/**
 * GPIO_ReadPin
 */

GPIO_Pin_State_t GPIO_ReadPin(GPIO_RegDef_t *Port, uint8_t pin)
{
    if (GET_GPIO_INDEX(Port) > 8 || pin > 15)
    {
        return;
    }
    return (Port->IDR & GPIO_PIN_MASK(pin)) ? GPIO_PIN_HIGH : GPIO_PIN_LOW;
}

/**
 * Configure Pull-up down
 */
void GPIO_SetPinPull(GPIO_RegDef_t *Port, uint8_t pin, GPIO_Pull_t pull)
{
    if (GET_GPIO_INDEX(Port) > 8 || pin > 15)
    {
        return;
    }

    Port->PUPDR &= ~(3U << (pin * 2));
    Port->PUPDR |= ((uint32_t) pull & 3U) << (pin *2 );
}

/**
 * @param Port: GPIO (A, B, C, ...)
 * @param pin: Pin (0-15)
 * @param af: Value of function (0 a 15)
 */
void GPIO_SetPinAlternateFunction(GPIO_RegDef_t *Port, uint8_t pin, uint8_t af)
{
    
    if (GET_GPIO_INDEX(Port) > 8 || pin > 15 || af > 15)
    {
        return;
    }

    uint8_t afr_idndex = pin >> 3;
    Port->AFR[afr_idndex] &= ~(0xFU << ((pin & 7) * 4));
    Port->AFR[afr_idndex] |= ((uint32_t)(af & 0xFU) << ((pin & 7) * 4));
}

/**
 * @details Configure the IO Output Speed
 * @param Port: GPIO (A, B, C, ...)
 * @param pin: Pin (0-15)
 * @param speed: enum (Low, Medium, High, Very High)
 */
void GPIO_SetPinOutputSpeed(GPIO_RegDef_t *Port, uint8_t pin, GPIO_Pin_Output_Speed_t speed)
{
    if (GET_GPIO_INDEX(Port) > 8 || pin > 15)
    {
        return;
    }

    Port->OSPEEDR &= ~(3U << (pin *2));
    Port->OSPEEDR |= ((uint32_t) speed & 3U) << (pin *2 );

}

/**
 * Configure the I/O Output type.
 * @param Port: GPIO (A, B, C, ...)
 * @param pin: Pin (0-15)
 * @param otype: enum (Push-pull, Open-drain)
 */
void GPIO_SetPinOutputType(GPIO_RegDef_t *Port, uint8_t pin, GPIO_Output_Type_t otype)
{
    if (GET_GPIO_INDEX(Port) > 8 || pin > 15)
    {
        return;
    }

    if (otype == GPIO_OUTPUT_OPEN_DRAIN)
    {
        Port->OTYPER |= (1U << pin);
    }
    else
    {
        Port->OTYPER &= ~(1U << pin);
    }

}

/**
 * 
 */
void GPIO_Init(GPIO_Pin_Config_t *Gpiox)
{
    GPIO_Clock_Enable(Gpiox->Port);
    
}

/**
 * Write to entire GPIO port (all 16 pins)
 */
void GPIO_WritePort(GPIO_RegDef_t *Port, uint16_t value)
{
    Port->ODR = (uint32_t)value;
}

void GPIO_SetPort(GPIO_RegDef_t *Port, uint16_t mask)
{
    Port->BSRR = (uint32_t) mask;
}
/**
 * Read entire GPIO port (all 16 pins)
 */
uint16_t GPIO_ReadPort(GPIO_RegDef_t *Port)
{
    return (uint16_t)Port->IDR;
}

void GPIO_ResetPort(GPIO_RegDef_t *Port, uint16_t mask)
{
    Port->BSRR = (uint16_t) mask << 16;
}






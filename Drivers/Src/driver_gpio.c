#include "driver_gpio.h"
#include "stm32l4xx.h"

/**
 * 
 */
uint32_t gpio_get_bank_base(gpio_bank_t bank)
{
    if (bank >= GPIO_BANK_COUNT)
    {
        return 0;
    }
    return GPIOA_BASE + (bank * GPIO_BANK_OFFSET);
}

/**
 * 
 */
void gpio_enable_bank(gpio_bank_t bank)
{
    if (bank >= GPIO_BANK_COUNT)
    {
        return;
    }
    GPIO_REG(RCC_BASE, RCC_AHB2ENR_OFFSET) |= (1U << bank);
    (void)GPIO_REG(RCC_BASE, RCC_AHB2ENR_OFFSET);
}

void gpio_mode_set(gpio_bank_t bank, uint8_t pin, gpio_mode_t mode) 
{
    if (bank >= GPIO_BANK_COUNT || pin > 15)
    {
        return;
    }

    GPIO_RegDef_t *gpio = (GPIO_RegDef_t *) gpio_get_bank_base(bank);
    gpio->MODER &= ~(3U << (pin * 2));  // clear previous mode
    gpio->MODER |= ((uint32_t)mode & 3) << (pin *2 ); // set new mode

}

/**
 * @brief Configures alternate function of a given pin
 * @param bank: Bank (A, B, C, ...)
 * @param pin: Pin (0-15)
 * @param alternate_func: Value of function (0 a 15)
 */

void gpio_alternate_set(gpio_bank_t bank, uint8_t pin, uint8_t alternate_func)
{
    if (bank >= GPIO_BANK_COUNT || pin > 15)
    {
        return;
    }

    GPIO_RegDef_t *gpio = (GPIO_RegDef_t *) gpio_get_bank_base(bank);
    // Each pin uses 4 bits in the AFR register
    
    uint8_t idx = pin >> 3;
    
    gpio->AFR[idx] &= ~(15UL << ((pin & 7) * 4));
    gpio->AFR[idx] |= ((uint32_t)alternate_func << ((pin & 7) * 4));
}

/**
 * 
 */
void gpio_pin_set(gpio_bank_t bank, uint8_t pin)
{
    if (bank >= GPIO_BANK_COUNT || pin > 15)
    {
        return;
    }
    GPIO_RegDef_t *gpio = (GPIO_RegDef_t *) gpio_get_bank_base(bank);
    // gpio->BSRR = GPIO_PIN_MASK(pin);
    gpio->ODR |= GPIO_PIN_MASK(pin);


}

void gpio_pin_clear(gpio_bank_t bank, uint8_t pin)
{   
    if (bank >= GPIO_BANK_COUNT || pin > 15)
    {
        return;
    }
    GPIO_RegDef_t *gpio = (GPIO_RegDef_t *) gpio_get_bank_base(bank);
    // gpio->BSRR = GPIO_PIN_MASK(pin) << 16U;
    gpio->ODR &= ~GPIO_PIN_MASK(pin);
    
}

/**
 * Pull-up/down configuration
 */
void gpio_pull_set(gpio_bank_t bank, uint8_t pin, gpio_pull_t pull) {

    if (bank >= GPIO_BANK_COUNT || pin > 15)
    {
        return;
    }

    GPIO_RegDef_t *gpio = (GPIO_RegDef_t *) gpio_get_bank_base(bank);
    gpio->PUPDR &=  ~(3U << (pin * 2));
    gpio->PUPDR |= ((uint32_t)pull & 3) << (pin * 2 );

}

void gpio_speed_set(gpio_bank_t bank, uint8_t pin, gpio_pin_speed_t speed)
{
    if (bank >= GPIO_BANK_COUNT || pin > 15)
    {
        return ;
    }

    GPIO_RegDef_t *gpio = (GPIO_RegDef_t *) gpio_get_bank_base(bank);
    gpio->OSPEEDR &=  ~(3U << (pin * 2));
    gpio->OSPEEDR |= ((uint32_t)speed & 3) << (pin * 2 );

}


void gpio_pin_toggle(gpio_bank_t bank, uint8_t pin) 
{
    if (bank >= GPIO_BANK_COUNT || pin > 15)
    {
       return;
    }

    GPIO_RegDef_t *gpio = (GPIO_RegDef_t *) gpio_get_bank_base(bank);
    
    gpio->ODR ^= (1U << pin);

}

void gpio_pin_write(gpio_bank_t bank, uint8_t pin, gpio_pin_state_t state) {

    if (state == GPIO_PIN_HIGH) {
        gpio_pin_set(bank, pin);
    }
    else {
        gpio_pin_clear(bank, pin);
    }
}

gpio_pin_state_t gpio_pin_read(gpio_bank_t bank, uint8_t pin) {
    GPIO_RegDef_t *gpio = (GPIO_RegDef_t *) gpio_get_bank_base(bank);
    return (gpio->IDR & GPIO_PIN_MASK(pin)) ? GPIO_PIN_HIGH : GPIO_PIN_LOW;
}
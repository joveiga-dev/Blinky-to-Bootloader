#include "driver_gpio.h"

/**
 * 
 */
uint32_t gpio_get_bank_base(gpio_bank_t bank)
{
    return GPIOA_BASE + (bank * GPIO_BANK_OFFSET);
}

void gpio_enable_bank(gpio_bank_t bank)
{
    GPIO_REG(RCC_BASE, RCC_AHB2ENR_OFFSET) |= (1U << bank);
    (void)GPIO_REG(RCC_BASE, RCC_AHB2ENR_OFFSET);
}

void gpio_mode_set(gpio_bank_t bank, uint8_t pin, gpio_mode_t mode) {
    GPIO_TypeDef *gpio = (GPIO_TypeDef *) gpio_get_bank_base(bank);
    gpio->MODER &= ~(3U << (pin * 2));  // clear previous mode
    gpio->MODER |= ((uint32_t)mode & 3) << (pin *2 ); // set new mode
}

void gpio_pin_set(gpio_bank_t bank, uint8_t pin)
{
    GPIO_TypeDef *gpio = (GPIO_TypeDef *) gpio_get_bank_base(bank);
    if (pin > 15)
    {
        return;
    }

    gpio->ODR |= GPIO_PIN_MASK(pin);

}

void gpio_pin_clear(gpio_bank_t bank, uint8_t pin)
{   
    GPIO_TypeDef *gpio = (GPIO_TypeDef *) gpio_get_bank_base(bank);
    if (pin > 15)
    {
        return;
    }

    gpio->ODR &= ~GPIO_PIN_MASK(pin);
}

void gpio_pull_set(gpio_bank_t bank, uint8_t pin, gpio_pull_t pull) {
    GPIO_TypeDef *gpio = (GPIO_TypeDef *) gpio_get_bank_base(bank);
    gpio->PUPDR &=  ~(3U << (pin * 2));
    gpio->PUPDR |= ((uint32_t)pull & 3) << (pin * 2 );
}


void gpio_pin_toggle(gpio_bank_t bank, uint8_t pin) {
    GPIO_TypeDef *gpio = (GPIO_TypeDef *) gpio_get_bank_base(bank);
    gpio->ODR ^= (1U << pin);
}

void gpio_pin_write(gpio_bank_t bank, uint8_t pin, gpio_pin_state_t state) {
    //GPIO_TypeDef *gpio = (GPIO_TypeDef *) gpio_get_bank_base(bank);
    if (state == GPIO_PIN_HIGH) {
        gpio_pin_set(bank, pin);
    }
    else {
        gpio_pin_clear(bank, pin);
    }
}

gpio_pin_state_t gpio_pin_read(gpio_bank_t bank, uint8_t pin) {
    GPIO_TypeDef *gpio = (GPIO_TypeDef *) gpio_get_bank_base(bank);
    return (gpio->IDR & GPIO_PIN_MASK(pin)) ? GPIO_PIN_HIGH : GPIO_PIN_LOW;
}
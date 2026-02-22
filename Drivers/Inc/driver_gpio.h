/**
 * STM32Lx GPIO Driver
 * 
 * The STM32Lx has 8 GPIO banks (GPIOA-GPIOH), each with 16 pins
 * Total: 
 *  
 */

 #ifndef __GPIO_H__
 #define __GPIO_H__

 #include <stdint.h>
 #include <stdbool.h>

/* Memory map for STM32L4 board
* Section 2.2.2 of the reference manual
*/
/* Base Addresses*/
#define FLASH_BASE                    (0x08000000U)
#define PERIPH_BASE                   (0x40000000U) // Same as APB1

/* Bus base addresses */
#define APB1_BASE              (PERIPH_BASE + 0x00000U)
#define APB2_BASE              (PERIPH_BASE + 0x10000U)
#define AHB1_BASE              (PERIPH_BASE + 0x20000U)
#define AHB2_BASE              (0x48000000U)

/* Bus APB1 */
/* Bus APB2 */

/* AHB1 Peripheral for enabling GPIO clocks*/
#define RCC_BASE                     (AHB1_BASE + 0x1000U)
#define RCC_AHB2ENR_OFFSET           (0x4CU)
/* GPIOA Bank */
#define GPIOA_BASE                   (AHB2_BASE + 0x0000U)
#define GPIO_BANK_OFFSET             (0x400U)

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;   // Output Type Register offset 0x04
    volatile uint32_t OSPEEDR;  // Output Speed Register offset 0x08
    volatile uint32_t PUPDR;    // Pull-up/Pull-down Register offset 0x0C
    volatile uint32_t IDR;      // Input Data Register offset 0x10
    volatile uint32_t ODR;      // Output Data Register  offset 0x14
    volatile uint32_t BSRR;     // Set/Reset Register offset 0x18
    volatile uint32_t LCKR;     // Configuration Lock Register
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

typedef enum {
    GPIO_BANK_A = 0,
    GPIO_BANK_B,
    GPIO_BANK_C,
    GPIO_BANK_D,
    GPIO_BANK_E,
    GPIO_BANK_F,
    GPIO_BANK_G,
    GPIO_BANK_H
} gpio_bank_t;

typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALTERNATE,
    GPIO_MODE_ANALOG
}gpio_mode_t;

typedef enum {
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN,
    GPIO_RESERVED
}gpio_pull_t;

typedef enum {
    GPIO_PIN_LOW =0,
    GPIO_PIN_HIGH
} gpio_pin_state_t;

typedef enum {
    GPIO_LOW_SPEED = 0,
    GPIO_MEDIUM_SPEED = 0,
    GPIO_HIGH_SPEED = 0,
    GPIO_VERY_HIGH_SPEED
} gpio_pin_speed_t;


/* Helper macros*/
#define GPIO_REG(base, offset)          (*(volatile uint32_t *)((base) + (offset)))
#define GPIO_PIN_MASK(pin)              (1U << (pin))

/* Function Prototypes*/

void gpio_enable_bank(gpio_bank_t bank);
uint32_t gpio_get_bank_base(gpio_bank_t bank);
void gpio_mode_set(gpio_bank_t bank, uint8_t pin, gpio_mode_t mode);
void gpio_pull_set(gpio_bank_t bank, uint8_t pin, gpio_pull_t pull);
gpio_mode_t gpio_get_mode(gpio_bank_t bank, uint8_t pin);

/* Digital IO*/
void gpio_pin_set(gpio_bank_t bank, uint8_t pin);
void gpio_pin_clear(gpio_bank_t bank, uint8_t pin);
void gpio_pin_toggle(gpio_bank_t bank, uint8_t pin);
void gpio_pin_write(gpio_bank_t bank, uint8_t pin, gpio_pin_state_t state);
gpio_pin_state_t gpio_pin_read(gpio_bank_t bank, uint8_t pin);

/* Port operations (all 16 pins at once)*/
void gpio_port_write(gpio_bank_t bank, uint16_t value);
uint16_t gpio_port_read(gpio_bank_t bank);
//void gpio_port_set_bits(gpio_bank_t bank, uint16_t mask);
//void gpio_port_clear_bits(gpio_bank_t bank, uint16_t mask);


#endif /* GPIO_H */
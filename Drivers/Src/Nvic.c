#include "Nvic.h"
#include "Stm32l4xx.h"

/**
 * Enable Interrupt
 */
void NVIC_EnableIRQ(IRQn_Type_t IRQn)
{
    uint8_t reg_index = IRQn >> 5;      // 32
    uint8_t bit_position = IRQn & 0x1F; // % 32

    NVIC->NVIC_ISER[reg_index] = (1U << bit_position);
}

void NVIC_DisableIRQ(IRQn_Type_t IRQn)
{
    uint8_t reg_index = IRQn >> 5;      // 32
    uint8_t bit_position = IRQn & 0x1F; // % 32

    NVIC->NVIC_ICER[reg_index] = (1U << bit_position);
}
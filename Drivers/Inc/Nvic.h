#ifndef NVIC_H_
#define NVIC_H_

typedef enum
{
    USART1_IRQn = 37,
    USART2_IRQn = 38
} IRQn_Type_t;

void NVIC_EnableIRQ(IRQn_Type_t IRQn);
void NVIC_DisableIRQ(IRQn_Type_t IRQn);

#endif
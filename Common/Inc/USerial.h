#ifndef USERIAL_H_
#define USERIAL_H_

#include <stdint.h>
#include "Stm32l4xx.h"
#include "Usart.h"

/* GPIO Configuration for USART*/
typedef struct 
{
    GPIO_RegDef_t *TX_Port;
    uint8_t tx_pin;
    GPIO_RegDef_t *RX_Port;
    uint8_t rx_pin;
    uint8_t af;
} USART_GPIO_Config_t;

typedef struct 
{
    USART_Config_t Usartx;
    USART_GPIO_Config_t Gpiox;
} USART_Handle;

void Userial_Init(USART_Handle *Husart);
void Userial_WriteChar(USART_Handle *Husart, char c);
void Userial_WriteString(USART_Handle *Husart, const char *str);


#endif
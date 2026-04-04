#ifndef USERIAL_H_
#define USERIAL_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

#include "Stm32l4xx.h"
#include "Usart.h"

#define USART2_PORT        (GPIOA)
#define BR_USART2          (9600)
#define TX_PIN_USART2      (GPIO_PIN_2)
#define RX_PIN_USART2      (GPIO_PIN_3)
#define AF_USART2          (7)

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

// APIs
void Userial_Init(USART_Handle *Husart);
void Userial_SendByte(USART_Handle *Husart, uint8_t data);
void Userial_SendData(USART_Handle *Husart, const uint8_t *data, uint32_t len);
void Userial_SendString(USART_Handle *Husart, const char *data);
uint32_t Userial_ReceiveData(uint8_t *data, uint32_t len);
uint8_t Userial_ReceiveByte(void);
void Userial_PollReceive(USART_Handle *Husart);
bool Userial_Data_Available(void);
void Userial_Debug(USART_Handle *huart, const char *fmt, ...);
void Userial_DebugTask(USART_Handle *huart);

#endif
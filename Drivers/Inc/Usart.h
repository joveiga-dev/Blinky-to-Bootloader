/**
 * driver_uart.h
 * 
 * Created on: Fev, 2026
 * @author: JV
 * 
 */

#ifndef DRIVER_UART_H_
#define DRIVER_UART_H_

#include <stdint.h>
#include <stdio.h>
#include "stm32l4xx.h"

typedef enum {
    PARITY_NONE = 0,
    PARITY_EVEN,
    PARITY_ODD
} USART_Parity_t;

typedef enum {
    WORDLENGTH_BIT_8,
    WORDLENGTH_BIT_9,
    WORDLENGTH_BIT_7
} USART_Word_Length_t;

typedef enum {
    STOPBITS_1,
    STOPBITS_0_5,
    STOPBITS_2,
    STOPBITS_1_5
} USART_Stop_Bits_t;

typedef enum {
    OVERSAMPLING_16 = 0,
    OVERSAMPLING_8
} USART_Oversampling_t;

typedef enum {
    MODE_TX,
    MODE_RX,
    MODE_TX_RX
} USART_Mode_t;

typedef struct
{
    USART_RegDef_t *Port;
    uint32_t baud_rate;          
    USART_Mode_t mode;                   
    USART_Word_Length_t data_bits;              
    USART_Stop_Bits_t stop_bits;              
    USART_Parity_t parity;                
    USART_Oversampling_t oversampling;
} USART_Config_t;

/* API prototypes */
int USART_Init(USART_Config_t *Usartx);
void USART_Clock_Enable(USART_Config_t *Usartx);
void USART_Clock_Disable(USART_Config_t *UsartX);

int USART_SetWordLength(USART_RegDef_t *Usartx, USART_Word_Length_t len);
int USART_SetParity(USART_RegDef_t *Usartx, USART_Parity_t par);
int USART_SetStopBits(USART_RegDef_t *Usartx, USART_Stop_Bits_t stop);
int USART_SetOversampling(USART_RegDef_t *Usartx, USART_Oversampling_t over8);

uint32_t USART_Calc_BRR(uint32_t pclk, uint32_t baud, uint8_t over);

void USART_SendByte(USART_RegDef_t *Usartx, uint8_t byte);
uint8_t USART_ReceiveByte(USART_RegDef_t *Usartx);

#endif
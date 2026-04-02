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
#include <stdbool.h>
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
    MODE_DISABLED,
    MODE_TX,
    MODE_RX,
    MODE_TX_RX
} USART_Mode_t;

typedef enum {
    HW_CONTROL_NONE,
    HW_CONTROL_RTS,
    HW_CONTROL_CTS,
    HW_CONTROL_RTS_CTS
}USART_Hw_Flow_Control_t;

typedef struct
{
    USART_RegDef_t *Port;
    uint32_t baud_rate;
    USART_Word_Length_t DataBits; 
    USART_Stop_Bits_t StopBits;              
    USART_Parity_t Parity;          
    USART_Mode_t Mode;
    USART_Hw_Flow_Control_t HwFlowCtl;                                                
    USART_Oversampling_t OverSampling;
} USART_Config_t;

/* API prototypes */
int USART_Init(USART_Config_t *Usartx);
void USART_Clock_Enable(USART_Config_t *Usartx);
void USART_Clock_Disable(USART_Config_t *Usartx);
void USART_Enable(USART_Config_t *Usartx);
void USART_Disable(USART_Config_t *Usartx);
void USART_EnableRxInterrupt(USART_Config_t *Usartx);

int USART_SetWordLength(USART_RegDef_t *Usartx, USART_Word_Length_t DataBits);
int USART_SetParity(USART_RegDef_t *Usartx, USART_Parity_t Parity);
int USART_SetStopBits(USART_RegDef_t *Usartx, USART_Stop_Bits_t StopBits);
int USART_SetOversampling(USART_RegDef_t *Usartx, USART_Oversampling_t OverSampling);
int USART_SetMode(USART_RegDef_t *Usartx, USART_Mode_t Mode);
int USART_SetFlowControl(USART_RegDef_t *Usartx, USART_Hw_Flow_Control_t FlowControl);

uint32_t USART_Calc_BRR(uint32_t pclk, uint32_t baud, uint8_t over);

void USART_SendFrame(USART_RegDef_t *Usartx, uint16_t data);
uint16_t USART_ReceivePollFrame(USART_RegDef_t *Usartx);
uint16_t USART_ReceiveFrame(USART_RegDef_t *Usartx);
bool USART_RxIsReady(USART_RegDef_t *Usartx);


#endif
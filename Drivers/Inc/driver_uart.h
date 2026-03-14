
#ifndef __DRIVER_UART_H__
#define __DRIVER_UART_H__

#include <stdint.h>
#include <stdio.h>
#include "driver_gpio.h"
#include "stm32l4xx.h"

#define PERIPHERAL_CLOCK        40000000
#define BR_UART_2               9600
#define UART2_BUFFER_LENGTH      1024



typedef struct {
    volatile uint32_t CR1;     // 0x00
    volatile uint32_t CR2;     // 0x04
    volatile uint32_t CR3;     // 0x08
    volatile uint32_t BRR;     // 0x0C
    volatile uint32_t GTPR;    // 0x10
    volatile uint32_t RTOR;    // 0x14
    volatile uint32_t RQR;     // 0x18
    volatile uint32_t ISR;     // 0x1C
    volatile uint32_t ICR;     // 0x20
    volatile uint32_t RDR;     // 0x24
    volatile uint32_t TDR;     // 0x28
} UART_RegDef_t;

typedef enum {
    UART_1 = 0,
    UART_2,
    UART_3,
    UART_4,
    UART_5
} UART_t;

#define BIT(x)     ((1UL) << (x))
#define USART2     ((UART_RegDef_t *) USART2_BASE)

void uart_config(UART_RegDef_t *uart, uint32_t baud_rate);
void uart_write_string(const char *buffer);
void uart_write_char(char c);
void uart_send_byte(unsigned char byte);


#endif
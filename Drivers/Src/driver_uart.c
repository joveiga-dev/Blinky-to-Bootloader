#include "driver_uart.h"
#include "stm32l4xx.h"
#include "driver.rcc.h"
#include "driver_systick.h"


void uart_config(UART_RegDef_t *uart, uint32_t baud_rate)
{   
    if (uart == USART2)
    {
        RCC->APB1ENR1 |= BIT(17);
    }
        
    gpio_mode_set(GPIO_BANK_A, 2, GPIO_MODE_ALTERNATE); // tx pa2
    gpio_alternate_set(GPIO_BANK_A, 2, 7);

    gpio_mode_set(GPIO_BANK_A, 3, GPIO_MODE_ALTERNATE); // rx pa3
    gpio_alternate_set(GPIO_BANK_A, 3, 7);
        
    uart->CR1 = 0;  // Disable UART
    uart->BRR = CLOCK_FREQ / baud_rate;
    uart-> CR1 |= BIT(0) | BIT(2) | BIT (3); // UE , RE, TE


}

void uart_write_char(char c)
{
    while (!(USART2->ISR & (1 << 7)));  // Wait TXE
    USART2->TDR = c;
}

void uart_write_string(const char *buffer)
{  

    while (*buffer != 0)
    {
        uart_write_char(*buffer++);
    }
}
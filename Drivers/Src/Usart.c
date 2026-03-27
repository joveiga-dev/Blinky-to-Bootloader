#include "Usart.h"
#include "Gpio.h"
#include "Stm32l4xx.h"
#include "Rcc.h"
#include "SystickTimer.h"

void GPIO_USART_Config(GPIO_USART_Config_t *gpiox)
{   
    GPIO_Clock_Enable(gpiox->Port);
    GPIO_SetPinMode(gpiox->Port, gpiox->tx_pin, GPIO_MODE_ALTERNATE);
    GPIO_SetPinAlternateFunction(gpiox->Port, gpiox->tx_pin, gpiox->alternate_func);



    if(gpiox->Port == GPIO_BANK_A)
    {
        gpio_enable_bank(GPIO_BANK_A);
        // Configure Tx
        GPIO_SetPinMode();
        gpio_mode_set(GPIO_BANK_A, gpiox->tx_pin, GPIO_MODE_ALTERNATE);
        gpio_alternate_set(GPIO_BANK_A, gpiox->tx_pin, gpiox->alternate_func);
        gpio_pull_set(GPIO_BANK_A, gpiox->tx_pin, GPIO_PULL_NONE);
        // Configure Rx
        gpio_mode_set(GPIO_BANK_A, gpiox->rx_pin, GPIO_MODE_ALTERNATE);
        gpio_alternate_set(GPIO_BANK_A, gpiox->rx_pin, gpiox->alternate_func);
        gpio_pull_set(GPIO_BANK_A, gpiox->rx_pin, GPIO_PULL_UP);
    }
    else if (gpiox->Port == GPIO_BANK_B)
    {
        gpio_enable_bank(GPIO_BANK_B);
    }

    
}

/**
 * @brief USART_Set_Word_Length
 */
void USART_Set_Word_Length(USART_RegDef_t *config, USART_Word_Length_t len)
{   
    /* 1. Clear old configuration */
    config->USART_CR1 &= ~(USART_CR1_M0 | USART_CR1_M1);
    /* 2. Set new configuration */
    switch (len)
    {
    case WORDLENGTH_BIT_8:
        /* M1M0 = 00 */
        break;
    case WORDLENGTH_BIT_9:
        config->USART_CR1 |= USART_CR1_M0;
        break;
    case WORDLENGTH_BIT_7:
        config->USART_CR1 |= USART_CR1_M1;
        break;
    }
}

/**
 * @brief USART_Set_Parity - Translates into Hardware
 * @details Configure Parity Enable (PCE) , Parity Type (PS = even/odd)
 * The USART must be disabled before configuring (UE = 0)
 */
void USART_Set_Parity(USART_RegDef_t *usartx, USART_Parity_t par)
{
    /* 1. Disable USART_UE first */
    usartx->USART_CR1 &= ~USART_CR1_UE;
    /* 2. Clear Parity bits */
    usartx->USART_CR1 &= ~(USART_CR1_PCE | USART_CR1_PS);
    /* 3. Apply new configuration */
    switch (par)
    {
    case PARITY_NONE:
        /* PCE 0 PS X */
        break;
    case PARITY_EVEN:
        usartx->USART_CR1 |= USART_CR1_PCE;
        break;
    case PARITY_ODD:
        usartx->USART_CR1 |= (USART_CR1_PCE | USART_CR1_PS);
        break;
    }
}

/**
 * @brief USART_Set_Stop_Bits 
 * @details The USART must be disabled before configuring (UE = 0)
 */
void USART_Set_Stop_Bits(USART_RegDef_t *usartx, USART_Stop_Bits_t stop)
{
    usartx->USART_CR1 &= ~USART_CR1_UE;

    usartx->USART_CR2 &= ~(USART_CR2_STOP);
    switch (stop)
    {
    case STOPBITS_1:
        break;
    case STOPBITS_0_5:
        usartx->USART_CR2 |= USART_CR2_STOP_0; 
        break;
    case STOPBITS_2:
        usartx->USART_CR2 |= USART_CR2_STOP_1; 
        break;
    case STOPBITS_1_5:
        usartx->USART_CR2 |= USART_CR2_STOP; 
        break;
    }
}

/**
 * @details The USART must be disabled before configuring (UE = 0)
 */
void USART_SetOversampling(USART_RegDef_t *usartx, USART_Oversampling_t over8)
{
    usartx->USART_CR1 &= ~USART_CR1_UE;
    if (over8 == OVERSAMPLING_16)
    {
        usartx->USART_CR1 &= ~USART_CR1_OVER8;
    }
    else
    {
        usartx->USART_CR1 |= USART_CR1_OVER8;
    }
}


void USART_Clock_Enable(USART_Config_t *config)
{
    if (config->Port == LPUART1)
    {
        RCC->RCC_APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;
    }
    else if (config->Port == USART1)
    {
        RCC->RCC_APB2ENR |= RCC_APB2ENR_USART1EN;
    }
    else if (config->Port == USART2)
    {
        RCC->RCC_APB1ENR1 |= RCC_APB1ENR1_USART2EN;
    }
    else if (config->Port == USART3)
    {
        RCC->RCC_APB1ENR1 |= RCC_APB1ENR1_USART3EN;
    }
    else if (config->Port == UART4)
    {
        RCC->RCC_APB1ENR1 |= RCC_APB1ENR1_UART4EN;
    }
    else if (config->Port == UART5)
    {
        RCC->RCC_APB1ENR1 |= RCC_APB1ENR1_UART5EN;
    }
    else
    {
        return;
    }
}

/**
 * @brief USART_Clock_Disable
 */
void USART_Clock_Disable(USART_Config_t *config)
{
    if (config->Port == LPUART1)
    {
        RCC->RCC_APB1ENR2 &= ~RCC_APB1ENR2_LPUART1EN;
    }
    else if (config->Port == USART1)
    {
        RCC->RCC_APB2ENR &= ~RCC_APB2ENR_USART1EN;
    }
    else if (config->Port == USART2)
    {
        RCC->RCC_APB1ENR1 &= ~RCC_APB1ENR1_USART2EN;
    }
    else if (config->Port == USART3)
    {
        RCC->RCC_APB1ENR1 &= ~RCC_APB1ENR1_USART3EN;
    }
    else if (config->Port == UART4)
    {
        RCC->RCC_APB1ENR1 &= ~RCC_APB1ENR1_UART4EN;
    }
    else if (config->Port == UART5)
    {
        RCC->RCC_APB1ENR1 &= ~RCC_APB1ENR1_UART5EN;
    }
    else
    {
        return;
    }
}

uint32_t USART_Calc_BRR(uint32_t pclk, uint32_t baud, uint8_t over8)
{
    uint32_t usartdiv;
    if (over8 == 0)
    {
        // OVER16

    }

}

void USART_Init(USART_Config_t *usartx)
{
    USART_Clock_Enable(usartx);

    usartx->Port->USART_CR1 &= ~USART_CR1_UE;

    USART_Set_Parity(usartx->Port, usartx->parity);
    USART_Set_Word_Length(usartx->Port, usartx->data_bits);
    USART_Set_Stop_Bits(usartx->Port, usartx->stop_bits);

    usartx->Port->USART_CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;

}


void uart_config(USART_RegDef_t *uart, uint32_t baud_rate)
{   
    if (uart == USART2)
    {
        RCC->APB1ENR1 |= BIT(17);
    }
        
    gpio_mode_set(GPIO_BANK_A, 2, GPIO_MODE_ALTERNATE); // tx pa2
    gpio_alternate_set(GPIO_BANK_A, 2, 7);

    gpio_mode_set(GPIO_BANK_A, 3, GPIO_MODE_ALTERNATE); // rx pa3
    gpio_alternate_set(GPIO_BANK_A, 3, 7);
        
    uart->USART_CR1 = 0;  // Disable UART
    uart->USART_BRR = CLOCK_FREQ / baud_rate;
    uart-> USART_CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE; // UE , RE, TE


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
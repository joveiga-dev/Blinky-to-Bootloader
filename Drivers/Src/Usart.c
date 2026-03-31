#include "Usart.h"
#include "Gpio.h"
#include "Stm32l4xx.h"
#include "Rcc.h"
#include "SysTick.h"

/**
 * @brief Enable Clock of the Usart
 */
void USART_Clock_Enable(USART_Config_t *Usartx)
{
    if (Usartx->Port == LPUART1)
    {
        RCC->RCC_APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;
    }
    else if (Usartx->Port == USART1)
    {
        RCC->RCC_APB2ENR |= RCC_APB2ENR_USART1EN;
    }
    else if (Usartx->Port == USART2)
    {
        RCC->RCC_APB1ENR1 |= RCC_APB1ENR1_USART2EN;
    }
    else if (Usartx->Port == USART3)
    {
        RCC->RCC_APB1ENR1 |= RCC_APB1ENR1_USART3EN;
    }
    else if (Usartx->Port == UART4)
    {
        RCC->RCC_APB1ENR1 |= RCC_APB1ENR1_UART4EN;
    }
    else if (Usartx->Port == UART5)
    {
        RCC->RCC_APB1ENR1 |= RCC_APB1ENR1_UART5EN;
    }
    else
    {
        return;
    }
}

/**
 * @brief @brief Enable Clock of the Usart
 */
void USART_Clock_Disable(USART_Config_t *Usartx)
{
    if (Usartx->Port == LPUART1)
    {
        RCC->RCC_APB1ENR2 &= ~RCC_APB1ENR2_LPUART1EN;
    }
    else if (Usartx->Port == USART1)
    {
        RCC->RCC_APB2ENR &= ~RCC_APB2ENR_USART1EN;
    }
    else if (Usartx->Port == USART2)
    {
        RCC->RCC_APB1ENR1 &= ~RCC_APB1ENR1_USART2EN;
    }
    else if (Usartx->Port == USART3)
    {
        RCC->RCC_APB1ENR1 &= ~RCC_APB1ENR1_USART3EN;
    }
    else if (Usartx->Port == UART4)
    {
        RCC->RCC_APB1ENR1 &= ~RCC_APB1ENR1_UART4EN;
    }
    else if (Usartx->Port == UART5)
    {
        RCC->RCC_APB1ENR1 &= ~RCC_APB1ENR1_UART5EN;
    }
    else
    {
        return;
    }
}

/**
 * @brief USART_Set_Word_Length
 */
int USART_SetWordLength(USART_RegDef_t *Usartx, USART_Word_Length_t len)
{   
    if (!Usartx || len > 3) return -1;

    Usartx->USART_CR1 &= ~(USART_CR1_M0 | USART_CR1_M1);
    switch (len)
    {
    case WORDLENGTH_BIT_8:
        /* M1M0 = 00 */
        break;
    case WORDLENGTH_BIT_9:
        Usartx->USART_CR1 |= USART_CR1_M0;
        break;
    case WORDLENGTH_BIT_7:
        Usartx->USART_CR1 |= USART_CR1_M1;
        break;
    }

    return 0;
}

/**
 * @brief USART_Set_Parity
 * @details Configure Parity Enable (PCE) , Parity Type (PS = even/odd)
 * The USART must be disabled before configuring (UE = 0)
 */
int USART_SetParity(USART_RegDef_t *Usartx, USART_Parity_t par)
{
    if (!Usartx || par > 4) return -1;

    //Usartx->USART_CR1 &= ~USART_CR1_UE;
    Usartx->USART_CR1 &= ~(USART_CR1_PCE | USART_CR1_PS);
    
    switch (par)
    {
    case PARITY_NONE:
        /* PCE 0 PS X */
        break;
    case PARITY_EVEN:
        Usartx->USART_CR1 |= USART_CR1_PCE;
        break;
    case PARITY_ODD:
        Usartx->USART_CR1 |= (USART_CR1_PCE | USART_CR1_PS);
        break;
    }

    return 0;
}

/**
 * @brief USART_Set_Stop_Bits 
 * @details The USART must be disabled before configuring (UE = 0)
 */
int USART_SetStopBits(USART_RegDef_t *Usartx, USART_Stop_Bits_t stop)
{
    if (!Usartx || stop > 4) return -1;

    //Usartx->USART_CR1 &= ~USART_CR1_UE;
    Usartx->USART_CR2 &= ~(USART_CR2_STOP);
    switch (stop)
    {
    case STOPBITS_1:
        break;
    case STOPBITS_0_5:
        Usartx->USART_CR2 |= USART_CR2_STOP_0; 
        break;
    case STOPBITS_2:
        Usartx->USART_CR2 |= USART_CR2_STOP_1; 
        break;
    case STOPBITS_1_5:
        Usartx->USART_CR2 |= USART_CR2_STOP; 
        break;
    }

    return 0;
}

/**
 * @brief Oversampling Uart settings
 * @details The USART must be disabled before configuring (UE = 0)
 */
int USART_SetOversampling(USART_RegDef_t *Usartx, USART_Oversampling_t over8)
{
    if (!Usartx || over8 > 2) return -1;

    //Usartx->USART_CR1 &= ~USART_CR1_UE;
    if (over8 == OVERSAMPLING_16)
    {
        Usartx->USART_CR1 &= ~USART_CR1_OVER8;
    }
    else
    {
        Usartx->USART_CR1 |= USART_CR1_OVER8;
    }

    return 0;
}


uint32_t USART_Calc_BRR(uint32_t pclk, uint32_t baud, uint8_t over)
{
    if (over == 0)
    {
        // OVER16
        return (pclk + (baud / 2U)) / baud;
    }
    else
    {
        // OVER8
        return (2U * pclk + (baud / 2U)) / baud;
    }
}

int USART_Init(USART_Config_t *Usartx)
{
    if (!Usartx || !Usartx->Port) return -1;

    USART_Clock_Enable(Usartx);
    Usartx->Port->USART_CR1 &= ~USART_CR1_UE; // Disable Uart

    // Configure Settings
    USART_SetParity(Usartx->Port, Usartx->parity);
    USART_SetWordLength(Usartx->Port, Usartx->data_bits);
    USART_SetStopBits(Usartx->Port, Usartx->stop_bits);
    USART_SetOversampling(Usartx->Port, Usartx->oversampling);
    
    // TODO: Replace with real clock when implemented Rcc Driver
    uint32_t pclk = 4000000;
    Usartx->Port->USART_BRR = USART_Calc_BRR(pclk, Usartx->baud_rate, Usartx->oversampling);

    Usartx->Port->USART_CR1 |= USART_CR1_RE | USART_CR1_TE;

    Usartx->Port->USART_CR1 |= USART_CR1_UE;

    return 0;
}


void USART_SendByte(USART_RegDef_t *Usartx, uint8_t byte)
{
    if(!Usartx ) return;

    while(!(Usartx->USART_ISR & USART_ISR_TXE));

    // Write data to transmit register
    Usartx->USART_TDR = byte;
}

uint8_t USART_ReceiveByte(USART_RegDef_t *Usartx)
{
    // Wait for receive data register not empty
    while(!(Usartx->USART_ISR & USART_ISR_RXNE));
    // Read data
    return (uint8_t)(Usartx->USART_RDR & 0xFF);
}




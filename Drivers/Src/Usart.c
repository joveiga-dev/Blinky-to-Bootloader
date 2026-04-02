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

void USART_Enable(USART_Config_t *Usartx)
{
    if ((Usartx == NULL) || (Usartx->Port == NULL))
    {
        return;
    }

    Usartx->Port->USART_CR1 |= USART_CR1_UE;
}

void USART_Disable(USART_Config_t *Usartx)
{
    if ((Usartx == NULL) || (Usartx->Port == NULL))
    {
        return;
    }

    Usartx->Port->USART_CR1 &= ~USART_CR1_UE;
}

/**
 * @brief USART_Set_Word_Length
 */
int USART_SetWordLength(USART_RegDef_t *Usartx, USART_Word_Length_t DataBits)
{   
    if (!Usartx || DataBits > 3) return -1;

    Usartx->USART_CR1 &= ~(USART_CR1_M0 | USART_CR1_M1);
    switch (DataBits)
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
int USART_SetParity(USART_RegDef_t *Usartx, USART_Parity_t Parity)
{
    if (!Usartx || Parity > 4) return -1;

    //Usartx->USART_CR1 &= ~USART_CR1_UE;
    Usartx->USART_CR1 &= ~(USART_CR1_PCE | USART_CR1_PS);
    
    switch (Parity)
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
int USART_SetStopBits(USART_RegDef_t *Usartx, USART_Stop_Bits_t StopBits)
{
    if (!Usartx || StopBits > 4) return -1;

    Usartx->USART_CR2 &= ~(USART_CR2_STOP);
    switch (StopBits)
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
int USART_SetOversampling(USART_RegDef_t *Usartx, USART_Oversampling_t OverSampling)
{
    if (!Usartx || OverSampling > 2) return -1;

    //Usartx->USART_CR1 &= ~USART_CR1_UE;
    if (OverSampling == OVERSAMPLING_16)
    {
        Usartx->USART_CR1 &= ~USART_CR1_OVER8;
    }
    else
    {
        Usartx->USART_CR1 |= USART_CR1_OVER8;
    }

    return 0;
}

/**
 * @brief USART Transfer Mode
 */
int USART_SetMode(USART_RegDef_t *Usartx, USART_Mode_t Mode)
{
    if(!Usartx || Mode > 4) return -1;

    Usartx->USART_CR2 &= ~(USART_CR1_TE | USART_CR1_RE);

    switch (Mode)
    {
    case MODE_DISABLED:
        break;
    case MODE_TX:
        Usartx->USART_CR1 |= USART_CR1_TE;
        break;
    case MODE_RX:
        Usartx->USART_CR2 |= USART_CR1_RE; 
        break;
    case MODE_TX_RX:
        Usartx->USART_CR2 |= (USART_CR1_TE | USART_CR1_RE); 
        break;
    }

    return 0;

}

/**
 * @details Hw_Flow_Control UART Hardware Flow Control
 */
int USART_SetFlowControl(USART_RegDef_t *Usartx, USART_Hw_Flow_Control_t FlowControl)
{
    if(!Usartx || FlowControl > 4) return -1;

    Usartx->USART_CR3 &= ~(USART_CR3_CTSE | USART_CR3_RTSE);

    switch (FlowControl)
    {
    case HW_CONTROL_NONE:
        break;
    case HW_CONTROL_RTS:
        Usartx->USART_CR1 |= USART_CR3_RTSE;
        break;
    case HW_CONTROL_CTS:
        Usartx->USART_CR2 |= USART_CR3_CTSE; 
        break;
    case HW_CONTROL_RTS_CTS:
        Usartx->USART_CR2 |= (USART_CR3_CTSE | USART_CR3_RTSE); 
        break;
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
    USART_SetWordLength(Usartx->Port, Usartx->DataBits);
    USART_SetStopBits(Usartx->Port, Usartx->StopBits);
    USART_SetParity(Usartx->Port, Usartx->Parity);
    USART_SetMode(Usartx->Port, Usartx->Mode);
    USART_SetFlowControl(Usartx->Port, Usartx->HwFlowCtl);
    USART_SetOversampling(Usartx->Port, Usartx->OverSampling);
    
    // TODO: Replace with real clock when implemented Rcc Driver
    uint32_t pclk = CPU_FREQ;
    Usartx->Port->USART_BRR = USART_Calc_BRR(pclk, Usartx->baud_rate, Usartx->OverSampling);

    Usartx->Port->USART_CR1 |= USART_CR1_RE | USART_CR1_TE;

    //Usartx->Port->USART_CR1 |= USART_CR1_UE;
    USART_Enable(Usartx);

    return 0;
}

/**
 * @brief Sends a single byte over Uart
 * 
 * This function writes on byte to the Uart data register. It waits for the transmit data register to be empty (TXE flag) 
 * 
 * @param Usartx     Pointer to USART peripheral.
 * @param byte       Byte to be transmitted.
 * 
 * @note Low-level helper function used by higher-level functions.
 * 
 * @retval None
 */
void USART_SendFrame(USART_RegDef_t *Usartx, uint16_t data)
{
    if((Usartx == NULL) || (data == 0U))
    {
        return;
    }

    while(!(Usartx->USART_ISR & USART_ISR_TXE));

    Usartx->USART_TDR = data;
}

/**
 * @brief Receives a single byte over Uart
 * 
 * This function reads on byte to the Uart data register. It waits for the receive data register not empty (RXNE flag) 
 * 
 * @param Usartx     Pointer to USART peripheral.
 * 
 * @note Low-level helper function used by higher-level functions.
 * 
 * @retval None
 */
uint16_t USART_ReceiveFrame(USART_RegDef_t *Usartx)
{
    if(Usartx == NULL)
    {
        return 0;
    }

    while(!(Usartx->USART_ISR & USART_ISR_RXNE));

    return (uint16_t)(Usartx->USART_RDR & 0xFF);
}




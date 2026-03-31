#include "USerial.h"
#include "Gpio.h"
#include "Usart.h"

static void USART_GPIO_Init (USART_Handle *Husart)
{
    GPIO_Pin_Config_t tx = {
        .Port = Husart->Gpiox.TX_Port,
        .pin = Husart->Gpiox.tx_pin,
        .Mode = GPIO_MODE_ALT,
        .Pull = GPIO_PULL_NONE,
        .Speed = GPIO_HIGH_SPEED,
        .Otype = GPIO_OTYPE_PUSH_PULL,
        .Alternate = Husart->Gpiox.af
    };

    GPIO_Pin_Config_t rx = {
        .Port = Husart->Gpiox.RX_Port,
        .pin = Husart->Gpiox.rx_pin,
        .Mode = GPIO_MODE_ALT,
        .Pull = GPIO_PULL_NONE,
        .Speed = GPIO_HIGH_SPEED,
        .Otype = GPIO_OTYPE_PUSH_PULL,
        .Alternate = Husart->Gpiox.af
    };

    GPIO_Init(&tx);
    GPIO_Init(&rx);
}

void Userial_Init(USART_Handle *Husart)
{
    if(!Husart) return;
    USART_GPIO_Init(Husart);

    USART_Config_t cfg = {
        .Port = Husart->Usartx.Port,
        .baud_rate = Husart->Usartx.baud_rate,
        .mode = Husart->Usartx.mode,
        .data_bits = Husart->Usartx.data_bits,
        .stop_bits = Husart->Usartx.stop_bits,
        .parity = Husart->Usartx.parity,
        .oversampling = Husart->Usartx.oversampling
    };

    USART_Init(&cfg);
}

void Userial_WriteChar(USART_Handle *Husart, char c)
{
    if(!Husart) return;

    USART_SendByte(Husart->Usartx.Port, (uint8_t)c);
}

void Userial_WriteString(USART_Handle *Husart, const char *str)
{
    if(!Husart || !str) return;
    while (*str)
    {
        USART_SendByte(Husart->Usartx.Port, (uint8_t)(*str));
        str++;
    }
    
}



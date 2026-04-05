#include "USerial.h"
#include "Gpio.h"
#include "Usart.h"
#include "RingBuffer.h"

#define RING_BUFFER_SIZE                           (128)         // Must be power of 2
static RingBuffer_t Rb = {0U};
static uint8_t usart_rx_buffer[RING_BUFFER_SIZE] = {0U};

volatile uint32_t rx_irq_count = 0;
volatile uint32_t rx_overflow_count = 0;
volatile uint8_t rx_last_byte = 0;


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

    RingBuffer_Init(&Rb, usart_rx_buffer, RING_BUFFER_SIZE);

    USART_GPIO_Init(Husart);

    USART_Config_t cfg = {
        .Port = Husart->Usartx.Port,
        .baud_rate = Husart->Usartx.baud_rate,
        .DataBits = Husart->Usartx.DataBits,
        .StopBits = Husart->Usartx.StopBits,
        .Parity = Husart->Usartx.Parity,
        .Mode = Husart->Usartx.Mode,
        .HwFlowCtl = Husart->Usartx.HwFlowCtl,
        .OverSampling = Husart->Usartx.OverSampling
    };

    USART_Init(&cfg);
}


void Userial_SendByte(USART_Handle *Husart, uint8_t data)
{
    if((Husart == NULL) || (Husart->Usartx.Port == NULL))
    {
        return;
    }

    USART_SendFrame(Husart->Usartx.Port, (uint16_t) data);
}

/**
 * @brief Sends a Buffer of Data over Uart
 * 
 * This function transmits a number of bytes from a data buffer through the Uart peripheral. 
 * It uses a blocking method, meaning it waits until each byte is transmitted before sending the next one.
 * 
 * @param Usartx     Pointer to USART peripheral.
 * @param data       Pointer to the data buffer to be transmitted.
 * @param len        Number of bytes to transmit.
 * 
 * @warning Ensure that 'data' is valid and 'len' is correct to avoid memory issues.
 * 
 * @note TODO: Try to call USART_SendByte(Husart, data[n]) and check if the compiler will inline to avoid function calls overhead
 * 
 * @retval None
 */
void Userial_SendData(USART_Handle *Husart, const uint8_t *data, uint32_t len)
{
    if((Husart == NULL) || (Husart->Usartx.Port == NULL) || (data == NULL) || (len == 0U))
    {
        return;
    }

    USART_RegDef_t *usart = Husart->Usartx.Port;
    
    for (uint32_t l = 0U; l < len; l++)
    {
        USART_SendFrame(usart, (uint16_t)data[l]);
    }
}


/**
 * @brief Sends a string over Uart
 * 
 * This function transmits characters until '\0' is reached.
 * 
 * @param Usartx     Pointer to USART peripheral.
 * @param data       Pointer to the string
 * 
 * @warning Ensure the buffer must be null-terminated
 * 
 * @retval None
 */
void Userial_SendString(USART_Handle *Husart, const char *data)
{
    if((Husart == NULL) || (Husart->Usartx.Port == NULL) || (data == NULL))
    {
        return;
    }

    USART_RegDef_t *usart = Husart->Usartx.Port;

    while (*data != '\0')
    {
        USART_SendFrame(usart, (uint16_t)(*data));
        data++;
    }
    
}

/**
 * @details Interrupt Service Routine
 */

 void USART2_IRQHandler(void)
 {
    
    if(USART_RxIsReady(USART2))
    {   
        rx_irq_count++;

        uint8_t frame = (uint8_t)USART_ReceiveFrame(USART2);
        rx_last_byte = frame;

        RingBuffer_Status_t status = RingBuffer_Write(&Rb, frame);
        
        if (status == RINGBUFFER_FULL)
        {
            //rx_overflow_count++;
        }
    }
 }

 /**
  * 
  */
uint32_t Userial_ReceiveData(uint8_t *data, uint32_t len)
{
    if ((len == 0U) || (data == NULL))
    {
        return 0;
    }

    uint32_t bytes_read = 0;

    for (bytes_read = 0; bytes_read < len; bytes_read++) 
    {
        if (RingBuffer_Read(&Rb, &data[bytes_read]) != RINGBUFFER_OK) 
        {
            break;
        }
    }

    return bytes_read;
}

/**
 * 
 */
uint8_t Userial_ReceiveByte(void)
{
    uint8_t byte = 0;
    if(RingBuffer_Read(&Rb, &byte) != RINGBUFFER_OK)
    {
        return 0;
    }
    return byte;
}

/**
 * 
 */
bool Userial_Data_Available(void)
{
    return (!RingBuffer_Empty(&Rb));
}

/**
 * 
 */
void Userial_Debug(USART_Handle *huart, const char *fmt, ...)
{
    char buf[128];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    Userial_SendString(huart, buf);
}

/**
 * 
 */
void Userial_DebugTask(USART_Handle *huart)
{
    uint8_t byte;

    if (RingBuffer_Read(&Rb, &byte) == RINGBUFFER_OK)
    {
        Userial_Debug(huart,
                   "RX byte: 0x%02X | IRQ=%lu | OVF=%lu\r\n",
                   byte,
                   rx_irq_count,
                   rx_overflow_count);
                   Userial_Debug(huart, "RX byte: 0x%02X\r\n", byte);
    }
}



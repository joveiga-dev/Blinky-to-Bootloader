#include "USerial.h"
#include "SysTick.h"
#include "Led.h"
#include "Stm32l4xx.h"
#include "Comms.h"
#include "Nvic.h"
#include "Common_Defines.h"
#include "Bootloader.h"

/* Private variables */
USART_Handle huart2;

/**
 * Jump to Application Function
 */
void JumpToApp(void)
{

    uint32_t MSP_entry = *(volatile uint32_t *)APP_START;
    uint32_t Reset_entry = *(volatile uint32_t *)(APP_START + 4U);

#ifdef DEBUG_MODE
    char aux_buffer[64];
    memset(aux_buffer, 0, sizeof(aux_buffer));
    sprintf(aux_buffer, "JumpToApp ...\r\n");
    Userial_SendArray(&huart2, (uint8_t *)aux_buffer, strlen(aux_buffer));

    Delay_Ms(5);

#endif

    /// disable USART
    USART_Disable(&huart2.Usartx);

    // Disable all interrupts
    for (IRQn_Type_t irq = 0; irq < 100; irq++)
    {
        NVIC_DisableIRQ(irq);
    }

    if ((MSP_entry < SRAM_BASE) || (MSP_entry > (SRAM_END)))
    {
        while (1)
            ;
        Led_On(LED2);
    }

    void (*AppResetHandler)(void);
    AppResetHandler = (void (*)(void))Reset_entry;
    AppResetHandler();
}

/**
 * Application Initialization Function
 */
void App_Init(void)
{
    Led_InitAllLeds();
    SysTick_Init(SYSTICK_LOAD_1MS);
    Usart2_Init();

#ifdef DEBUG_MODE
    char aux_buffer[64];
    memset(aux_buffer, 0, sizeof(aux_buffer));
    sprintf(aux_buffer, "Bootloader version: %d.%d\r\n", MAJOR_VERSION, MINOR_VERSION);
    Userial_SendArray(&huart2, (uint8_t *)aux_buffer, strlen(aux_buffer));

    Delay_Ms(5);

#endif
}

/**
 * Application Task Function
 */
void App_Task(void)
{
    // Send packet to PC
    Comms_Packet_t packet = {
        .length = 9,
        .data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
        .crc = 0};
    // Calculate CRC
    packet.crc = Comms_Compute_CRC(&packet);

    while (true)
    {
        // Comms_Update(&huart2);
        Comms_Send(&huart2, &packet);
        Delay_Ms(1000);
    }

    JumpToApp();
}

/**
 * USART2 Initialization Function
 */
void Usart2_Init(void)
{
    huart2.Usartx.Port = USART2;
    huart2.Usartx.baud_rate = BR_USART2;
    huart2.Usartx.DataBits = WORDLENGTH_BIT_8;
    huart2.Usartx.StopBits = STOPBITS_1;
    huart2.Usartx.Parity = PARITY_NONE;
    huart2.Usartx.Mode = MODE_TX_RX;
    huart2.Usartx.HwFlowCtl = HW_CONTROL_NONE;
    huart2.Usartx.OverSampling = OVERSAMPLING_16;

    huart2.Gpiox.TX_Port = GPIOA;
    huart2.Gpiox.tx_pin = TX_PIN_USART2;

    huart2.Gpiox.RX_Port = GPIOA;
    huart2.Gpiox.rx_pin = RX_PIN_USART2;

    huart2.Gpiox.af = AF_USART2;

    Userial_Init(&huart2);
}

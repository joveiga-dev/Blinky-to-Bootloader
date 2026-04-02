/**
 * @author: José Veiga
 * @details contains everything necessary to boot and jump to main()
 */

#include <stdint.h>

#define SRAM_BASE        0X20000000U             // Data memory
#define SRAM_SIZE        (96 * 1024) // 96Kbytes
#define SRAM_END         ((SRAM_BASE) + (SRAM_SIZE))   // 0x20018000

#define STACK_TOP       (SRAM_END)

/* Linker Symbols*/
extern uint32_t _sbss, _ebss;
extern uint32_t _sdata, _edata;
extern uint32_t _etext;

int main(void);
void Reset_Handler(void);
void Default_Handler(void);

typedef void (*Interrupt_Handler)(void);

void NMI_Handler                             (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler                       (void) __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler                       (void) __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler                        (void) __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler                      (void) __attribute__ ((weak, alias("Default_Handler")));
void SVCall_Handler                          (void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMonitor_Handler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler                          (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler                         (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI16_PVD_IRQHandler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void TAMP_STAMP_IRQHandler        	(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI22_RTC_WKUP_IRQHandler   	(void) __attribute__ ((weak, alias("Default_Handler")));
void FLASH_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void RCC_IRQHandler               	(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI0_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI1_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI2_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI3_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI4_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream0_IRQHandler      	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream1_IRQHandler      	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream2_IRQHandler      	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream3_IRQHandler      	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream4_IRQHandler      	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream5_IRQHandler      	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream6_IRQHandler      	(void) __attribute__ ((weak, alias("Default_Handler")));
void ADC_IRQHandler               	(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler           	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_BRK_TIM9_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_UP_TIM10_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_TRG_COM_TIM11_IRQHandler	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler           	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM2_IRQHandler              	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM3_IRQHandler              	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM4_IRQHandler              	(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler           	(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler           	(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler           	(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler           	(void) __attribute__ ((weak, alias("Default_Handler")));
void SPI1_IRQHandler              	(void) __attribute__ ((weak, alias("Default_Handler")));
void SPI2_IRQHandler              	(void) __attribute__ ((weak, alias("Default_Handler")));
void USART1_IRQHandler            	(void) __attribute__ ((weak, alias("Default_Handler")));
void USART2_IRQHandler            	(void) __attribute__ ((weak, alias("Default_Handler")));         
/**
 * Initial interrupt vector table 
 */
__attribute__((section(".vectors"), used))
const Interrupt_Handler vectors[] = {
    (Interrupt_Handler) STACK_TOP,                   /* &_esram Top of stack 96K */
    Reset_Handler,                                   /* Reset Handler */
    NMI_Handler,                                     /* NMI */
    HardFault_Handler,                               /* Hard Fault */
    MemManage_Handler,                               /* MemManage */
    BusFault_Handler,                                /* BusFault */
    UsageFault_Handler,                              /* UsageFault*/
    0,                                               /* Reserved 7 */
    0,                                               /* Reserved 8  */
    0,                                               /* Reserved 9  */
    0,                                               /* Reserved 10  */
    SVCall_Handler,                                  /* SVCall */
    DebugMonitor_Handler,                            /* Debug */
    0,                                               /* Reserved 13 */
    PendSV_Handler,                                  /* PendSV */
    SysTick_Handler,                                  /* SysTick */

    /* External Interrupts */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    USART2_IRQHandler

};

/**
 * Reset Handler code. This will be executed when the MCU comes out of reset
 */
void Reset_Handler(void)
{
    // memset .bss to zero
    for(uint32_t *p_dst = &_sbss; p_dst < &_ebss; p_dst++)
    {
        *p_dst = 0;
    }

    // Copy .data from Flash to RAM
    for(uint32_t *p_dst = &_sdata, *p_src = &_etext; p_dst < &_edata;)
    {
        *p_dst++ = *p_src++;
    }
    main();   // Call main
    while (1) (void) 0;   // Infinite loop in case if main() returns
}

// When Fault occurs. The device will only recover in this situation if it's manually reset or runs 
// out of power. 
// . It’s generally a good idea to make sure all exception handlers at least reboot the device when 
// a fault occurs to give the device a chance to recover
void Default_Handler(void)
{
    while (1) (void) 0;
}




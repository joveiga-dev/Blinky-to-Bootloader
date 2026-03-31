#include "SysTick.h"
#include "Stm32l4xx.h"
#include <stdint.h>

volatile uint32_t systick_ms = 0;
//static void (*SysTick_Callback)(void) = 0;

/**
 * @brief    system tick init
 * @details  Initializes the System timer and its interrupt, and starts the system tick timer.
 * @param    [ticks] ticks number of ticks between 
 * @return   0  function succeeded
 * @return   1  function failed
 */
int SysTick_Init(uint32_t reload)
{
    if ((reload -1UL) > SYSTICK_RVR_RELOAD_Msk) // 24-bit
    {
        return -1;
    }

    SYSTICK->SYST_RVR = (reload -1UL); 
    SYSTICK->SYST_CVR = 0UL; 

    SYSTICK->SYST_CSR = SYSTICK_CSR_CLKSOURCE | SYSTICK_CSR_TICKINT | SYSTICK_CSR_ENABLE;

    return 0;
}

void SysTick_Start(void)
{
    SYSTICK->SYST_CSR |=  SYSTICK_CSR_ENABLE;
}

void SysTick_Stop(void)
{
    SYSTICK->SYST_CSR &=  ~SYSTICK_CSR_ENABLE;
}

/**
 * @brief Increments each ms
 */

void SysTick_Handler(void)
{
    systick_ms++; 
}

/**
 * @brief Get the Current Time
 */
uint32_t SysTick_GetTick(void)
{
    uint32_t tick;
    //__disable_irq();
    tick = systick_ms;
    //__enable_irq();
    return tick;
}

uint32_t SysTick_GetTimeMs(void)
{
    return SysTick_GetTick();
}

uint32_t SysTick_GetTimeSec(void)
{
    return SysTick_GetTimeMs() / 1000U;
}

/**
 * 
 */
void Delay_Ms(uint32_t ms)
{
    uint32_t start = systick_ms;
    while ((systick_ms - start) < ms)
    {
        __asm volatile ("wfi");
    }
    
}

uint8_t SysTick_Elapsed(uint32_t start, uint32_t delay)
{
    return ((systick_ms - start) >= delay);
}


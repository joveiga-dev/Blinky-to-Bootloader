#include "driver_systick.h"
#include "stm32l4xx.h"
#include <stdint.h>

volatile uint32_t systick_ticks = 0;

/**
 * @brief    system tick init
 * @details  Initializes the System timer and its interrupt, and starts the system tick timer.
 * @param    [ticks] ticks number of ticks between 
 * @return   0  function succeeded
 * @return   1  function failed
 */
void systick_timer_init(uint32_t ticks)
{
    if ((ticks -1UL) > SYSTICK_RVR_RELOAD_MASK) // 24-bit
    {
        return;
    }

    SYSTICK->SYST_RVR = (ticks -1UL); 
    SYSTICK->SYST_CVR = 0UL; 
    SYSTICK->SYST_CSR = SYSTICK_CSR_CLKSOURCE |
                        SYSTICK_CSR_TICKINT |
                        SYSTICK_CSR_ENABLE;
}


void SysTick_Handler(void)
{
    systick_ticks++;
}

/**
 * 
 */
uint32_t systicktimer_get_current_count(void)
{
    return systick_ticks;
}

/**
 * 
 */
void delay_ms(uint32_t milleseconds)
{
    uint32_t until = systick_ticks + milleseconds;
    while (systick_ticks < until)
    {
        (void) 0;
    }
    
}

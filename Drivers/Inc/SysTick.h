/**
 * The processor has a 24-bit system timer, SysTick, that counts down from the reload value to zero
 * 
 * 
 * Systick timer Driver
 * @author: José Veiga
 * @date: 27/02/2026
 * @details 
 *  
 */

#ifndef SYSTICKTIMER_H_
#define SYSTICKTIMER_H_

#include <stdint.h>
#include "Stm32l4xx.h"

#define CPU_FREQ        (4000000UL)
#define SYSTICK_FREQ    (1000UL)
#define SYSTICK_LOAD_1MS (CPU_FREQ / 1000UL)

//extern volatile uint32_t systick_ms;

/* APIs definitions */

int SysTick_Init(uint32_t reload);
void SysTick_Start(void);
void SysTick_Stop(void);

uint32_t SysTick_GetTick(void);
uint32_t SysTick_GetTimeMs(void);
uint32_t SysTick_GetTimeSec(void);

void Delay_Ms(uint32_t ms);
void Delay_Us(uint32_t us);

uint8_t SysTick_Elapsed(uint32_t start, uint32_t delay);
void SysTick_Handler(void);
void SysTick_EnableWDT(void);

#endif


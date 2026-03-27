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

#ifndef __DRIVER_SYSTICK_H__
#define __DRIVER_SYSTICK_H__

#include <stdint.h>

extern volatile uint32_t systick_ticks;

#define BIT_MASK(bit)     (1UL << (bit))
#define TIMER_REG(base, offset)    (*(volatile uint32_t *)((base) + (offset)))


typedef struct {
    volatile uint32_t SYST_CSR;      /* offset: 0x000 (R/W) SysTick Control and Status Register */
    volatile uint32_t SYST_RVR;      /* offset: 0x004 (R/W) SysTick Reload and Value Register */
    volatile uint32_t SYST_CVR;      /* offset: 0x008 (R/W) SysTick Current Value Register */
    volatile uint32_t SYST_CALIB;    /* offset 0x00C (R/ )  SysTick Calibration Value Register*/
} systick_reg_map_t;

typedef enum {
    TMR_BLINK_USER_LED,
    TMR_UART_DEBUG,

    TMR_MAX
} timer_timeout;


/* APIs definitions */
void timer_clear();

void systick_timer_init(uint32_t ticks);
void systicktimer_set_reload(uint32_t value);
uint32_t systicktimer_get_reload(void);
void systicktimer_enable(void);
void systicktimer_disable(void);

void delay_ms(uint32_t milleseconds);
void delay_us(uint32_t microseonds);

void systick_timer_set(uint8_t val);
uint32_t systicktimer_get_current_count(void);
uint32_t systicktimer_is_running(uint32_t base);

void SysTick_Handler(void);



#endif


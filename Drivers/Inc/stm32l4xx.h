#ifndef __STM32L476XX_H_
#define __STM32L476XX_H_

#include <stdint.h>

/* Memory map for STM32L4 board
* Section 2.2.2 of the reference manual
*/
/* Base Addresses*/
#define FLASH_BASE                    (0x08000000U)
#define PERIPH_BASE                   (0x40000000U) // Same as APB1

/* Bus base addresses */
#define APB1_BASE              (PERIPH_BASE + 0x00000U)
#define APB2_BASE              (PERIPH_BASE + 0x10000U)
#define AHB1_BASE              (PERIPH_BASE + 0x20000U)
#define AHB2_BASE              (0x48000000U)

/* Bus APB1 */
//#define APB1_BASE              (PERIPH_BASE)
//#define RCC_APB1ENR_OFFSET     (0x4CU)
/* Bus APB2 */

/* AHB1 Peripheral for enabling GPIO clocks*/
#define RCC_BASE                     (AHB1_BASE + 0x1000U)
#define RCC_AHB2ENR_OFFSET           (0x4CU)
/* GPIOA Bank */
#define GPIOA_BASE                   (AHB2_BASE + 0x0000U)
#define GPIO_BANK_OFFSET             (0x400U)


#define SCS_BASE                 (0xE000E000UL)
#define SYSTICK_BASE             (SCS_BASE + 0x0010UL)
#define SYSTICK                  ((systick_reg_map_t  *) SYSTICK_BASE)

/* Control / Status Register definitions */
#define SYSTICK_CSR_ENABLE       (BIT_MASK(0U))       /* Enables the counter: 0-> counter disabled, 1 counter enabled.*/
#define SYSTICK_CSR_CLKSOURCE    (BIT_MASK(2U))       /* Indicates the clock source: 0 external and 1 processor clock */
#define SYSTICK_CSR_COUNTFLAG    (BIT_MASK(16U))      /* Returns 1 if timer counted to 0 since last time this was read.*/
#define SYSTICK_CSR_TICKINT      (BIT_MASK(1U))       /* Enables SysTick exception request*/

/* Reload and Value Register definitions */
#define SYSTICK_RVR_RELOAD_MASK   (0XFFFFFFUL)
#define SYSTICK_RVR_RELOAD        (SYSTICK_RVR_RELOAD)   // Value to load into the CVR register when the counter is enabled and when it reaches 0

/* Current Value Register definitions */
#define SYSTICK_CVR_CURRENT     (SYSTICK_RVR_RELOAD_MASK)   /* Returns the current value of the systick counter CURRENT[23:0] */

/* SysTick Calibration Value Register*/
#define SYSTICK_CALIB_NOREF    BIT_MASK(31U)   /* indicates if the device provides a reference clock to the processor. 0 yes and 1 not*/
#define SYSTICK_CALIB_SKEW     BIT_MASK(30U) 
#define SYSTICK_CALIB_TENMS    (SYSTICK_RVR_RELOAD_MASK)


/* UART */
#define USART2_BASE        (APB1_BASE + 0x4400U)

#define SYS_FREQ           (80000000UL)
#define GetSystemClock     (SYS_FREQ)

#define CLOCK_FREQ        (4000000UL)
// Define a frequência do SysTick para 1kHz (1ms por tick)
#define TICKS_FOR_1MS (CLOCK_FREQ / 1000UL)
#define MS_TO_TICKS(ms) ((ms * (CLOCK_FREQ / 1000UL)))


#endif
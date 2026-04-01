#ifndef STM32L476XX_H_
#define STM32L476XX_H_

#include <stdint.h>

/* Base Addresses*/
#define FLASH_BASE                    (0x08000000U)      // Code memory
#define SRAM_BAsE                     (0x20000000U)      // Data memory
#define PERIPH_BASE                   (0x40000000U)      // Peripheral registers

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
#define RCC_AHB2ENR_OFFSET           (0x4CU)
#define SCS_BASE                 (0xE000E000UL)
#define SYSTICK_BASE             (SCS_BASE + 0x0010UL)

#define BIT(pos)                 ((0x1UL) << pos)

/**
 * @brief Reset Control Clock RCC
 */
typedef struct
{
  volatile uint32_t RCC_CR;                    /* Clock control register,           Address offset: 0x00 */
  volatile uint32_t RCC_ICSCR;
  volatile uint32_t RCC_CFGR;
  volatile uint32_t RCC_PLLCFGR;
  volatile uint32_t RCC_PLLSAI1CFGR;
  volatile uint32_t RCC_PLLSAI2CFGR;
  volatile uint32_t RCC_CIER;
  volatile uint32_t RCC_CIFR;
  volatile uint32_t RCC_CICR;
  uint32_t RESERVED0;
  volatile uint32_t RCC_AHB1RSTR;
  volatile uint32_t RCC_AHB2RSTR;
  volatile uint32_t RCC_AHB3RSTR;
  uint32_t RESERVED1;
  volatile uint32_t RCC_APB1RSTR1;
  volatile uint32_t RCC_APB1RSTR2;
  volatile uint32_t RCC_APB2RSTR;
  uint32_t RESERVED2;
  volatile uint32_t RCC_AHB1ENR;
  volatile uint32_t RCC_AHB2ENR;
  volatile uint32_t RCC_AHB3ENR;
  uint32_t RESERVED3;
  volatile uint32_t RCC_APB1ENR1;
  volatile uint32_t RCC_APB1ENR2;
  volatile uint32_t RCC_APB2ENR;            /* APB2 peripheral clock enable register,          Address offset: 0x60 */
  uint32_t RESERVED4;
  volatile uint32_t RCC_AHB1SMENR;
  volatile uint32_t RCC_AHB2SMENR;
  volatile uint32_t RCC_AHB3SMENR;
  uint32_t RESERVED5;           
  volatile uint32_t RCC_APB1SMENR1;       /* Address offset: 0x78 */
  volatile uint32_t RCC_APB1SMENR2;
  // .....

} RCC_RegDef_t;

#define RCC_BASE        (AHB1_BASE + 0x1000U)
#define RCC             ((RCC_RegDef_t *) RCC_BASE)


/**
 * @brief General Purpose Input Output (GPIO)
 */
typedef struct {
    volatile uint32_t MODER;           // GPIO Port mode register,            Address offset: 0x00
    volatile uint32_t OTYPER;          // GPIO Output Type Register           Address offset: 0x04
    volatile uint32_t OSPEEDR;  // Output Speed Register offset 0x08
    volatile uint32_t PUPDR;    // Pull-up/Pull-down Register offset 0x0C
    volatile uint32_t IDR;      // Input Data Register offset 0x10
    volatile uint32_t ODR;      // Output Data Register  offset 0x14
    volatile uint32_t BSRR;     // Set/Reset Register offset 0x18
    volatile uint32_t LCKR;     // Configuration Lock Register
    volatile uint32_t AFR[2];
} GPIO_RegDef_t;

#define GPIO_OFFSET                  (0x400U)                      /* 1024 bytes between Ports*/
#define GPIOA_BASE                   (AHB2_BASE + 0x0000U)
#define GPIOB_BASE                   (GPIOA_BASE + GPIO_OFFSET)
#define GPIOC_BASE                   (GPIOB_BASE + GPIO_OFFSET)
#define GPIOD_BASE                   (GPIOC_BASE + GPIO_OFFSET)
#define GPIOE_BASE                   (GPIOD_BASE + GPIO_OFFSET)
#define GPIOF_BASE                   (GPIOE_BASE + GPIO_OFFSET)
#define GPIOG_BASE                   (GPIOF_BASE + GPIO_OFFSET)
#define GPIOH_BASE                   (GPIOG_BASE + GPIO_OFFSET)
#define GPIOA                        ((GPIO_RegDef_t *) GPIOA_BASE)
#define GPIOB                        ((GPIO_RegDef_t *) GPIOB_BASE)
#define GPIOC                        ((GPIO_RegDef_t *) GPIOC_BASE)
#define GPIOD                        ((GPIO_RegDef_t *) GPIOD_BASE)
#define GPIOE                        ((GPIO_RegDef_t *) GPIOE_BASE)
#define GPIOF                        ((GPIO_RegDef_t *) GPIOF_BASE)
#define GPIOG                        ((GPIO_RegDef_t *) GPIOG_BASE)
#define GPIOH                        ((GPIO_RegDef_t *) GPIOH_BASE)
#define GET_GPIO_INDEX(port)         (((uint32_t)(port) - (uint32_t) GPIOA) / (uint32_t)GPIO_OFFSET)


#define RCC_AHB2ENR_GPIOHEN			(1 << 7)
#define RCC_AHB2ENR_GPIOGEN			(1 << 6)
#define RCC_AHB2ENR_GPIOFEN			(1 << 5)
#define RCC_AHB2ENR_GPIOEEN			(1 << 4)
#define RCC_AHB2ENR_GPIODEN			(1 << 3)
#define RCC_AHB2ENR_GPIOCEN			(1 << 2)
#define RCC_AHB2ENR_GPIOBEN			(1 << 1)
#define RCC_AHB2ENR_GPIOAEN			(1 << 0)

#define RCC_APB1ENR1_UART5EN		(1 << 20)
#define RCC_APB1ENR1_UART4EN		(1 << 19)
#define RCC_APB1ENR1_USART3EN	    (1 << 18)
#define RCC_APB1ENR1_USART2EN	    (1 << 17)
#define RCC_APB1ENR2_LPUART1EN      (1 << 0)    // Low power uart 1
#define RCC_APB2ENR_USART1EN        (1 << 14)
/**
 * @brief Universal Synchronous Asynchronous Receiver Transmitter (USART)
 */
typedef struct {
    volatile uint32_t USART_CR1;     // USART Control register 1,                    Address offset: 0x00
    volatile uint32_t USART_CR2;     // USART Control register 2,                    Address offset: 0x04
    volatile uint32_t USART_CR3;     // USART Control register 3,                    Address offset: 0x08
    volatile uint32_t USART_BRR;     // USART Baud rate register,                    Address offset: 0x0C
    volatile uint32_t USART_GTPR;    // USART Guard time and prescaler register,     Address offset: 0x10
    volatile uint32_t USART_RTOR;    // USART receiver timeout register.             Address offset: 0x14
    volatile uint32_t USART_RQR;     // USART request register,                      Address offset: 0x18
    volatile uint32_t USART_ISR;     // USART interrupt and status register          Address offset: 0x1C
    volatile uint32_t USART_ICR;     // USART interrupt flag clear register          Address offset: 0x20
    volatile uint32_t USART_RDR;     // USART receive data register                  Address offset: 0x24
    volatile uint32_t USART_TDR;     // USART transmit data register                 Address offset: 0x28
} USART_RegDef_t;

#define LPUART1_BASE       (APB1_BASE + 0x8000U)
#define USART1_BASE        (APB2_BASE + 0x3800U)
#define USART2_BASE        (APB1_BASE + 0x4400U)
#define USART3_BASE        (APB1_BASE + 0x4800U)
#define UART4_BASE         (APB1_BASE + 0x4C00U)
#define UART5_BASE         (APB1_BASE + 0x5000U)
#define LPUART1            ((USART_RegDef_t *) LPUART1_BASE)
#define USART1             ((USART_RegDef_t *) USART1_BASE)
#define USART2             ((USART_RegDef_t *) USART2_BASE)
#define USART3             ((USART_RegDef_t *) USART3_BASE)
#define UART4              ((USART_RegDef_t *) UART4_BASE)
#define UART5              ((USART_RegDef_t *) UART5_BASE)

/* USART Enable */
#define USART_CR1_UE            BIT(0U)
/* Tx Enable */
#define USART_CR1_TE            BIT(3U)
/*Rx Enable */               
#define USART_CR1_RE            BIT(2U)
/* USART Parity Selection (0- even, 1 - odd) */
#define USART_CR1_PS            BIT(9U)    
/* USART Parity Control Enable */
#define USART_CR1_PCE           BIT(10U)
/* USART Word Length */
#define USART_CR1_M0_Pos         (12U)
#define USART_CR1_M0_Msk         (0x1UL << USART_CR1_M0_Pos)
#define USART_CR1_M0             USART_CR1_M0_Msk
#define USART_CR1_M1_Pos         (28U)
#define USART_CR1_M1_Msk         (0x1UL << USART_CR1_M1_Pos)
#define USART_CR1_M1             USART_CR1_M1_Msk
#define USART_CR1_OVER8          BIT(15U)
/* USART baud rate register */
//#define USART_BRR_USART15_4

/* USART interrupt and status register */
/* RX enable acknowledge flag */
#define USART_ISR_REACK        BIT(22)
/* Tx enable acknowledge flag */
#define USART_ISR_TEACK        BIT(21)
/* Busy flag */
#define USART_ISR_BUSY         BIT(16)
/* Transmit data register empty */
#define USART_ISR_TXE          BIT(7)
/* Read data register not empty */
#define USART_ISR_RXNE         BIT(5)  

#define USART_CR2_STOP         (0x3U << 12)
#define USART_CR2_STOP_0       (0x1U << 12)
#define USART_CR2_STOP_1       (0x1U << 13)

/* Systick (system timer) Definitions */

#define SYSTICK_CSR_ENABLE       (BIT(0U))       /* Enables the counter: 0-> counter disabled, 1 counter enabled.*/
#define SYSTICK_CSR_CLKSOURCE    (BIT(2U))       /* Indicates the clock source: 0 external and 1 processor clock */
#define SYSTICK_CSR_COUNTFLAG    (BIT(16U))           /* Returns 1 if timer counted to 0 since last time this was read.*/
#define SYSTICK_CSR_TICKINT      (BIT(1U))       /* Enables SysTick exception request*/

/* Reload and Value Register definitions */
#define SYSTICK_RVR_RELOAD_Msk    (0XFFFFFFUL)
#define SYSTICK_RVR_RELOAD        (SYSTICK_RVR_RELOAD_Msk)   // Value to load into the CVR register when the counter is enabled and when it reaches 0

/* Current Value Register definitions */
#define SYSTICK_CVR_CURRENT     (SYSTICK_RVR_RELOAD_Msk)   /* Returns the current value of the systick counter CURRENT[23:0] */

/* SysTick Calibration Value Register*/
#define SYSTICK_CALIB_NOREF    BIT(31U)   /* indicates if the device provides a reference clock to the processor. 0 yes and 1 not*/
#define SYSTICK_CALIB_SKEW     BIT(30U) 
#define SYSTICK_CALIB_TENMS    (SYSTICK_RVR_RELOAD_Msk)

/**
 * @brief Structure to access the System Timer (SysTick)
 */
typedef struct {
    volatile uint32_t SYST_CSR;      /* offset: 0x000 (R/W) SysTick Control and Status Register */
    volatile uint32_t SYST_RVR;      /* offset: 0x004 (R/W) SysTick Reload and Value Register */
    volatile uint32_t SYST_CVR;      /* offset: 0x008 (R/W) SysTick Current Value Register */
    volatile uint32_t SYST_CALIB;    /* offset 0x00C (R/ )  SysTick Calibration Value Register*/
} SysTick_RegDef_t;

#define SYSTICK                  ((SysTick_RegDef_t *) SYSTICK_BASE)


#define MMIO8(addr)              (*(volatile uint8_t *)(addr))
#define MMIO16(addr)             (*(volatile uint16_t *)(addr))
#define MMIO32(addr)             (*(volatile uint32_t *)(addr))
#define MMIO64(addr)             (*(volatile uint64_t *)(addr))

/**
 * System Control Block (SCB)
 */

#define SCB_BASE                  (0xE000ED00UL)
#define SCB_VTOR_OFFSET           MMIO32(SCB_BASE + 0x08U)
















#endif
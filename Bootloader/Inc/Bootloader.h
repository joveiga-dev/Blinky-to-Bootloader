#ifndef BOOTLOADER_H
#define BOOTLOADER_H

/* Private defines */
#define BOOT_SIZE (0x8000U)
#define BOOT_START (FLASH_BASE)
#define BOOT_END (BOOT_START + BOOT_SIZE - 1U)
#define APP_START (FLASH_BASE + BOOT_SIZE)
#define APP_END (FLASH_BASE + FLASH_SIZE - 1U)
#define SET_VECTOR_TABLE 1
#define MAJOR_VERSION 1
#define MINOR_VERSION 0
// #define DEBUG_MODE

/* Private function prototypes */
void Usart2_Init(void);
void App_Init(void);
void JumpToApp(void);
void App_Task(void);

#endif // BOOTLOADER_H
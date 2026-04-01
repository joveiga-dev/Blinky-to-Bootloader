#include "defines.h"
#include "Stm32l4xx.h"

#define BOOTLOADER_SIZE                (0x8000U) // 32KB
#define MAIN_APP_START_ADDRESS         (FLASH_BASE + BOOTLOADER_SIZE) // 0x08008000
const uint8_t data[0x4000] = {0};
void Jump_to_App(void);

int main(void)
{
    volatile uint8_t x = 0;
    for (uint32_t i = 0; i < 0x4000; i++)
    {
        x += data[i];
    }
    
    
    Jump_to_App();

    return 0;
}

/**
 * @details To jump to our application code, we need to know where the 'Reset_Handler' of the app is, and what stack pointer to load.
 */
void Jump_to_App(void)
{
    typedef void (*void_fn)(void);
    uint32_t* reset_vector_entry = (uint32_t*)(MAIN_APP_START_ADDRESS + 4U);
    uint32_t* reset_vector = (uint32_t*)(*reset_vector_entry);

    void_fn jump_fn = (void_fn)reset_vector;
    jump_fn();
}



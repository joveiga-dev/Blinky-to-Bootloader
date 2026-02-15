#include <stdint.h>

#define RAM_START       0X20000000U
#define RAM_SIZE        (96 * 1024) // 96Kbytes
#define RAM_END         ((RAM_START) + (RAM_SIZE))

/* Linker Symbols*/
extern uint32_t _sbss, _ebss;
extern uint32_t _sdata, _edata;
extern uint32_t _etext;

int main(void);
void Reset_Handler(void);
void Default_Handler(void);


/* Vector table placed at start of flash */
__attribute__((section(".vectors"), used))
void (*vectors[])(void) = {
    (void(*)(void))RAM_END,  // Initial stack pointer
    Reset_Handler             // Reset handler
};


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

    main();

    while (1);

}

void Default_Handler(void) {
    while (1);
}


#include <stdint.h>

#define RAM_START       0X20000000U
#define RAM_SIZE        (96 * 1024) // 96Kbytes
#define RAM_END         ((RAM_START) + (RAM_SIZE))   // 0x20018000

#define STACK_START     (RAM_END)

/* Linker Symbols*/
extern uint32_t _sbss, _ebss;
extern uint32_t _sdata, _edata;
extern uint32_t _etext;

int main(void);
void Reset_Handler(void);
void Default_Handler(void);

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

    //__asm volatile("MSR MSP, %0": : "r" (STACK_START): ); // stack reset

    main();   // Call main

    while (1) (void) 0;   // Infinite loop in case if main() returns

}

void Default_Handler(void) {
    while (1);
}

__attribute__((section(".vectors"), used))
void (*vectors[])(void) = {
    (void(*)(void))STACK_START,   // Initial stack pointer
    Reset_Handler             // Reset handler
};


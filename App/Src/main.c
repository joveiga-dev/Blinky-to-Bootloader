#include <stdint.h>

#define RCC_BASE    0x40021000U
#define GPIOA_BASE  0x48000000U

#define RCC_AHB2ENR (*(volatile uint32_t *)(RCC_BASE + 0x4C))
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR   (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

void delay(void) {
    for(volatile int i=0;i<100000;i++);
}

int main(void) {
    RCC_AHB2ENR |= (1U << 0);       // Enable GPIOA clock
    GPIOA_MODER &= ~(3U << (5*2)); // Clear PA5 mode
    GPIOA_MODER |=  (1U << (5*2)); // Set PA5 output

    while(1) {
        GPIOA_ODR ^= (1 << 5);     // Toggle PA5
        delay();
    }
}

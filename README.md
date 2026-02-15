# Blinky-to-Bootloader
## Intro

## Goals
1. Learn Fundamentals of programming a microcontroller
2. Baremetal (no RTOS)
3. Understand the setup and use of various key peripherals
4. Allow firmware over USB serial
5. Singed Firmware mechanism

## Prerequisits
We need to have the following installed and and added to the correct environment path.
- [GNU ARM Embedded tools](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
    - Verify installation using this prompt-> arm-none-eabi-gcc --version
- [GNU Make](https://www.gnu.org/software/make/)
- [OpenOCD debug tool](https://github.com/xpack-dev-tools/openocd-xpack/releases)

## Nucleo-Board


### Chip
> STM32L476RGTx

### Memory map

| Memory | Start Address | Size |
| :--- | :----: | ---: |
| Flash | 0x08000000 | 1024 Kbytes |
| SRAM1 | 0x20000000 | 96 Kbytes |
| SRAM2 | 0x20000000 | 32 Kbytes |


### MCU Boot and Vector Table
when the MCU boots, it reads a "vector table" from the beginning of flash memory. A vector table is essentially an array of function pointers, telling the CPU where to go when specific events (Exceptions or Interrupts). The 16 entries are reserved and common to all MCUs families and the rest are interrupt handlers for peripherals.

| Index | Offset | Description |
| :--- | :----: | ---: |
| 0 | 0x00 | Initial Main SP - the starting address of the Stack |
| 1 | 0x04 | Reset Handler - the entry point of the program |
| 2 | 0x08 | NMI Handler - Non-Maskable Interrupt (Critical errors)|
| 3 | 0x0C | HardFault - called when the code crashes |
| 4 | -- | -- |
| 16+ | 0x40+ | IQRs - device specific peripherals (GPIO, UART, Timer, ADC,e tc.)  |

Vector table for STM32L476 is documented in table. We can see that there are 82 peripheral handlers, in addition to the standard 16.





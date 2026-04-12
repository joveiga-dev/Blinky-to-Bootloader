/*
********************************************************
Author: jveiga1998@outlook.com
Date: 06-02-2026
@file       : linker.ld
@brief      : Linker Script for STM32L476RG Device
                1024KBytes FLASH
                96KBytes RAM
                32KBytes SRAM2
*********************************************************
*/

/* Define the Entry Point of the output file. */
ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08008000, LENGTH = 992K   /* 1024 - 32 KB for bootloader */
    SRAM  (rwx): ORIGIN = 0x20000000, LENGTH = 96K
}

_estack = ORIGIN(SRAM) + LENGTH(SRAM);

/* Sections */
SECTIONS
{   /* Interrupt vectors. The first two entries are more special as they old the initial stack pointer and the
     * initial program counter (Reset Handler).
     * Mark it as KEEP to prevent it from being garbage collected.
    */

    .text :
    {   
        . = ALIGN(4);
        KEEP(*(.isr_vector))  /* Interrupt vector table */
        . = ALIGN(4);
        *(.text*)     /* Program code */
        *(.rodata*)   /* Ready-only data*/
        . = ALIGN(4);
        _etext = .;   /* Define a gloabl symbol at the end of code section */
    } > FLASH
    
    /* Initialized data section into RAM
    * During boot, we have to copy from flash to ram at the provided locations.
    * _sdata and _edata will be used for that
    */
    _sidata = LOADADDR(.data);

    .data : /* AT (_etext) */
    {   
        . = ALIGN(4);
        _sdata = .;   /* Create a global symbol at data start*/
        *(.data*)
        *(.data)
        . = ALIGN(4);
        _edata = .;  /* Create a global symbol at data end*/
    } > SRAM AT > FLASH
    
    .bss :
    {
        _sbss = .;
        *(.bss*)     /* Define a global symbol at bss start*/
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;    /* Define a global symbol at bss end*/
    } > SRAM

    _end = .; 

   
   /* Create a symbol for the end of ram */
    _esram = ORIGIN(SRAM) + LENGTH(SRAM);          
}
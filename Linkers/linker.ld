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
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
    SRAM  (rwx): ORIGIN = 0x20000000, LENGTH = 96K
    /*SRAM2(rwx) : ORIGIN = 0x10000000, LENGTH = 32K*/
}

/* Sections */
SECTIONS
{
    .vectors :
    {   
        . = ALIGN(4);
        KEEP(*(.vectors))   /* Vector Code */
        . = ALIGN(4);
    } > FLASH

    .text :
    {   
        . = ALIGN(4);
        *(.text*)     /* Program code */
        *(.rodata*)   /* Ready-only data*/
        . = ALIGN(4);
        _etext = .;   /* Define a gloabl symbol at the end of code section */
    } > FLASH
    
    /* Initialized data section into RAM*/
    .data :
    {   
        . = ALIGN(4);
        _sdata = .;   /* Create a global symbol at data start*/
        *(.data*)
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

    _end = .;             /* For cmsis_gcc.h */
}
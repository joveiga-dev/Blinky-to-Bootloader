
/* Define the Entry Point of the output file. */
ENTRY(Reset_Handler)

_estack = ORIGIN(SRAM) + LENGTH(SRAM);

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 32K
    SRAM  (rwx): ORIGIN = 0x20000000, LENGTH = 96K
}


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

    _sidata = LOADADDR(.data);
    
    /* Initialized data section into RAM
    * During boot, we have to copy from flash to ram at the provided locations.
    * _sdata and _edata will be used for that
    */
    .data : /* AT (_etext) */
    {   
        . = ALIGN(4);
        _sdata = .;   /* Create a global symbol at data start*/
        *(.data*)

        . = ALIGN(4);
        _edata = .;  /* Create a global symbol at data end*/
    } > SRAM AT > FLASH
    
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)     /* Define a global symbol at bss start*/
        *(COMMON)

        . = ALIGN(4);
        _ebss = .;    /* Define a global symbol at bss end*/
    } > SRAM

    _end = .; 

    _boot_end  = ORIGIN(FLASH) + LENGTH(FLASH);
    _boot_size = LENGTH(FLASH);
   
        
}
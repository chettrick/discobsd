/*
 * Kernel linker script for STM32F469NIHX.
 */

/*
 * On STM32, a minimum of 128 kbytes of flash and 128 kbytes of RAM is needed.
 * Reserve for the kernel at least 128 kbytes of flash and 32 kbytes of RAM.
 * Reserve for userland at least 96 kbytes of RAM.
 */
MEMORY {
    FLASH  (rx)   : ORIGIN = 0x08000000, LENGTH = 1024K /* 0x0800 0000 - 0x080F FFFF */
    RAM    (rw!x) : ORIGIN = 0x10000000, LENGTH = 58K   /* 0x1000 0000 - 0x1000 E7FF */
    U0AREA (rw!x) : ORIGIN = 0x1000E800, LENGTH = 3K    /* 0x1000 E800 - 0x1000 F3FF */
    UAREA  (rw!x) : ORIGIN = 0x1000F400, LENGTH = 3k    /* 0x1000 F400 - 0x1000 FFFF */

    USERRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 112K  /* 0x2000 0000 - 0x2001 BFFF */
}

__kernel_flash_start    = ORIGIN(FLASH);
__kernel_flash_end      = ORIGIN(FLASH) + LENGTH(FLASH);
__kernel_data_start     = ORIGIN(RAM);
__kernel_data_end       = ORIGIN(RAM) + LENGTH(RAM);
__user_data_start       = ORIGIN(USERRAM);
__user_data_end         = ORIGIN(USERRAM) + LENGTH(USERRAM);

u0      = ORIGIN(U0AREA);
u       = ORIGIN(UAREA);
u_end   = ORIGIN(UAREA) + LENGTH(UAREA);
_estack = ORIGIN(UAREA) + LENGTH(UAREA);

ENTRY(Reset_Handler)

SECTIONS {
    .text : {
        KEEP(*(.isr_vector))
        . = ALIGN(8);
        *(.text*)
        . = ALIGN(8);
        *(.rodata*)
    } > FLASH

    .ARM.extab : {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    .ARM.exidx : {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH
    __etext = .;

    _sidata = .;
    .data : {
        __data_start__ = .;
        _sdata = .;
        *(.data*)
        . = ALIGN(8);
        __data_end__ = .;
        _edata = .;
    } > RAM AT > FLASH

    .bss : {
        . = ALIGN(8);
        __bss_start__ = .;
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end__ = .;
        _ebss = .;
    } > RAM
}

/*
 * Kernel memory linker script for STM32F407XG.
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

    USERRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 96K   /* 0x2000 0000 - 0x2001 7FFF */
}

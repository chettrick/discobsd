/*
 * Kernel memory linker script for STM32F412RETX.
 */

/*
 * On STM32, a minimum of 128 kbytes of flash and 128 kbytes of RAM is needed.
 * Reserve for the kernel at least 128 kbytes of flash and 32 kbytes of RAM.
 * Reserve for userland at least 96 kbytes of RAM.
 */
MEMORY {
    FLASH  (rx)   : ORIGIN = 0x08000000, LENGTH = 512K  /* 0x0800 0000 - 0x0807 FFFF */
    RAM    (rw!x) : ORIGIN = 0x20038000, LENGTH = 26K   /* 0x2003 8000 - 0x2003 E7FF */
    U0AREA (rw!x) : ORIGIN = 0x2003E800, LENGTH = 3K    /* 0x2003 E800 - 0x2003 F3FF */
    UAREA  (rw!x) : ORIGIN = 0x2003F400, LENGTH = 3k    /* 0x2003 F400 - 0x2003 FFFF */

    USERRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 96K   /* 0x2000 0000 - 0x2001 7FFF */
}

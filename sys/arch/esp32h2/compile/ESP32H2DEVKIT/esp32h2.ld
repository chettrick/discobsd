/*
 * esp32h2.ld -- Linker script for DiscoBSD/esp32h2 Phase 1.
 *
 * ESP32-H2 memory regions as seen by elf2image and the ROM bootloader:
 *
 *   IRAM (instruction bus, executable):
 *     0x40800000 - 0x4084FFFF  (320 KB)
 *     elf2image recognises this range as "IRAM" and copies it from flash.
 *
 *   DRAM (data bus, byte-accessible):
 *     0x40800000 - 0x4084FFFF  (same physical SRAM, data bus alias)
 *     elf2image recognises this range as "DRAM" for data segments.
 *
 * On ESP32-H2 IRAM and DRAM share the same physical address space
 * (unlike ESP32 classic). We place everything in IRAM so elf2image
 * produces one clean loadable segment, which the ROM copies to SRAM
 * and then jumps to the entry point.
 *
 * The ROM bootloader stack sits near the top of SRAM. We leave the
 * top 16 KB free for it and use the rest.
 */

OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
    /*
     * Entire usable HP SRAM, seen via instruction bus.
     * Leave top 16 KB for ROM bootloader stack.
     */
    IRAM (rwx) : ORIGIN = 0x40800000, LENGTH = (320K - 16K)
}

_STACK_SIZE = 0x2000;   /* 8 KB kernel stack, inside IRAM region */

SECTIONS
{
    /*
     * All sections go into IRAM -- one contiguous segment.
     * elf2image will recognise 0x408xxxxx as IRAM and load it correctly.
     */
    . = ORIGIN(IRAM);

    .text :
    {
        KEEP(*(.text.init))     /* _start must be first */
        *(.text .text.*)
        *(.rodata .rodata.*)
        *(.srodata .srodata.*)
        . = ALIGN(4);
        _etext = .;
    } > IRAM

    _data_load  = .;
    _data_start = .;

    .data :
    {
        *(.data .data.*)
        *(.sdata .sdata.*)
        . = ALIGN(4);
        _data_end = .;
    } > IRAM

    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_start = .;
        *(.bss .bss.*)
        *(.sbss .sbss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
    } > IRAM

    .stack (NOLOAD) :
    {
        . = ALIGN(16);
        _stack_bottom = .;
        . += _STACK_SIZE;
        _estack = .;
    } > IRAM

    PROVIDE(__global_pointer$ = _data_start + 0x800);

    /DISCARD/ :
    {
        *(.eh_frame)
        *(.eh_frame_hdr)
    }
}

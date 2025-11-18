/*
 * Linker script for LM3S6965: flash and SRAM region layout.
 *
 * This script places the vector table and startup code in flash and
 * arranges data/bss in SRAM. It is tightly coupled to the chip memory
 * sizes and must be adjusted when changing target device or RAM/Flash
 * layout.
 *
 * Derived from the LM3S6965 datasheet memory map.
 */

MEMORY {
        FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
        SRAM  (rw) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS {
        . = 0x00000000;
        .text : {
            KEEP(*(.vectors))
            *(.startup)
            *(.text)
            *(.rodata)
            . = ALIGN(4);
        } > FLASH
        
        .data : {
            . = ALIGN(4);
            _sram_sdata = .;
            *(.data);
            . = ALIGN(4);
            _sram_edata = .;
        } > SRAM AT > FLASH

        _flash_sdata = LOADADDR(.data);

        .bss :{
            . = ALIGN(4);
            _sram_sbss = .;
            *(.bss)
            *(COMMON)
            . = ALIGN(4);
            _sram_ebss = .;
        } > SRAM

        _sram_stacktop = ORIGIN(SRAM) + LENGTH(SRAM);
}

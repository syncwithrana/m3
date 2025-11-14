    .syntax unified
    .cpu cortex-m3
    .thumb

/* -----------------------------------------------------------
 * Vector Table
 * ----------------------------------------------------------- */

    .section .isr_vector, "a", %progbits
    .align 2
    .global _vector_table
_vector_table:
    .word _estack          /* Initial Stack Pointer (from linker) */
    .word Reset_Handler    /* Reset Handler */
    .word Default_Handler  /* NMI */
    .word Default_Handler  /* HardFault */
    .word Default_Handler  /* MemManage */
    .word Default_Handler  /* BusFault */
    .word Default_Handler  /* UsageFault */
    .word 0
    .word 0
    .word 0
    .word 0
    .word Default_Handler  /* SVCall */
    .word Default_Handler  /* DebugMon */
    .word 0
    .word Default_Handler  /* PendSV */
    .word Default_Handler  /* SysTick */

    /* External interrupts (not used yet) */
    .rept 32
    .word Default_Handler
    .endr

/* -----------------------------------------------------------
 * Default Handler — infinite loop
 * ----------------------------------------------------------- */
    .thumb_func
Default_Handler:
    b .

/* -----------------------------------------------------------
 * Reset Handler
 * ----------------------------------------------------------- */
    .section .text.Reset_Handler, "ax", %progbits
    .align 2
    .global Reset_Handler
Reset_Handler:

    /* Copy .data section from FLASH to RAM */
    ldr r0, =_sidata       /* src */
    ldr r1, =_sdata        /* dst */
    ldr r2, =_edata
1:
    cmp r1, r2
    itt lt
    ldrlt r3, [r0], #4
    strlt r3, [r1], #4
    blt 1b

    /* Zero .bss */
    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r3, #0
2:
    cmp r1, r2
    it lt
    strlt r3, [r1], #4
    blt 2b

    /* Jump to main */
    bl main

    /* If main returns, loop forever */
    b .

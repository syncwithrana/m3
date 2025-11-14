.syntax unified
.cpu cortex-m3
.thumb

/* Vector table */
.section .isr_vector, "a", %progbits
.global _vector_table
_vector_table:
    .word _estack            /* Initial stack pointer */
    .word Reset_Handler      /* Reset handler */
    .word NMI_Handler
    .word HardFault_Handler
    .word Default_Handler    /* MemManage */
    .word Default_Handler    /* BusFault */
    .word Default_Handler    /* UsageFault */
    .word 0
    .word 0
    .word 0
    .word 0
    .word Default_Handler    /* SVCall */
    .word Default_Handler    /* DebugMonitor */
    .word 0
    .word Default_Handler    /* PendSV */
    .word Default_Handler    /* SysTick */

    /* External interrupts (we won't use yet) */
    .rept 32
    .word Default_Handler
    .endr

/* Default handlers */
.thumb_func
NMI_Handler:
HardFault_Handler:
Default_Handler:
    b .
    
/* Reset Handler */
.section .text.Reset_Handler
.thumb_func
.global Reset_Handler
Reset_Handler:

    /* Copy .data from flash to RAM */
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata
1:
    cmp r1, r2
    ittt lt
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

    /* Call main() */
    bl main

    /* If main returns, loop */
3:
    b 3b

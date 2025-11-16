#include <stdint.h>
#include "irq.h"

/* Linker symbols for TI Stellaris LM3S6965 
 * defining start/end of various sections
 * using long as ANSI C guarentees long to be at least 4 bytes (32-bits))
 */
extern uint32_t _flash_sdata;
extern uint32_t _sram_sdata;
extern uint32_t _sram_edata;
extern uint32_t _sram_sbss;
extern uint32_t _sram_ebss;
extern uint32_t _sram_stacktop;

extern void main(void);
extern void uart0_irq_handler(void);
extern void _SysTick_Handler(void);
void _Reset_Handler(void);

// -----------------------------------------------------------------------------
// Weak default interrupt handlers
// -----------------------------------------------------------------------------

void _Unused_Handler(void)
{
    while (1);
}

/* Exception Handlers */
void _NMI_Handler(void)              __attribute__((weak, alias("_Unused_Handler")));
void _Hard_Fault_Handler(void)       __attribute__((weak, alias("_Unused_Handler")));
void _Memory_Mgmt_Handler(void)      __attribute__((weak, alias("_Unused_Handler")));
void _Bus_Fault_Handler(void)        __attribute__((weak, alias("_Unused_Handler")));
void _Usage_Fault_Handler(void)      __attribute__((weak, alias("_Unused_Handler")));
void _SVCall_Handler(void)           __attribute__((weak, alias("_Unused_Handler")));
void _Debug_Monitor_Handler(void)    __attribute__((weak, alias("_Unused_Handler")));
void _PendSV_Handler(void)           __attribute__((weak, alias("_Unused_Handler")));
void _SysTick_Handler(void)          __attribute__((weak, alias("_Unused_Handler")));

/* IRQ Handlers 16–59 */
void GPIOA_Handler(void)             __attribute__((weak, alias("_Unused_Handler")));
void GPIOB_Handler(void)             __attribute__((weak, alias("_Unused_Handler")));
void GPIOC_Handler(void)             __attribute__((weak, alias("_Unused_Handler")));
void GPIOD_Handler(void)             __attribute__((weak, alias("_Unused_Handler")));
void GPIOE_Handler(void)             __attribute__((weak, alias("_Unused_Handler")));
void UART0_Handler(void)             __attribute__((weak, alias("_Unused_Handler")));
void UART1_Handler(void)             __attribute__((weak, alias("_Unused_Handler")));
void SSI0_Handler(void)              __attribute__((weak, alias("_Unused_Handler")));
void I2C0_Handler(void)              __attribute__((weak, alias("_Unused_Handler")));
void PWMFAULT_Handler(void)          __attribute__((weak, alias("_Unused_Handler")));
void PWMGEN0_Handler(void)           __attribute__((weak, alias("_Unused_Handler")));
void PWMGEN1_Handler(void)           __attribute__((weak, alias("_Unused_Handler")));
void PWMGEN2_Handler(void)           __attribute__((weak, alias("_Unused_Handler")));
void QEI0_Handler(void)              __attribute__((weak, alias("_Unused_Handler")));
void ADC0SEQ0_Handler(void)          __attribute__((weak, alias("_Unused_Handler")));
void ADC0SEQ1_Handler(void)          __attribute__((weak, alias("_Unused_Handler")));
void ADC0SEQ2_Handler(void)          __attribute__((weak, alias("_Unused_Handler")));
void ADC0SEQ3_Handler(void)          __attribute__((weak, alias("_Unused_Handler")));
void WDT_Handler(void)               __attribute__((weak, alias("_Unused_Handler")));
void TIMER0A_Handler(void)           __attribute__((weak, alias("_Unused_Handler")));
void TIMER0B_Handler(void)           __attribute__((weak, alias("_Unused_Handler")));
void TIMER1A_Handler(void)           __attribute__((weak, alias("_Unused_Handler")));
void TIMER1B_Handler(void)           __attribute__((weak, alias("_Unused_Handler")));
void TIMER2A_Handler(void)           __attribute__((weak, alias("_Unused_Handler")));
void TIMER2B_Handler(void)           __attribute__((weak, alias("_Unused_Handler")));
void ACMP0_Handler(void)             __attribute__((weak, alias("_Unused_Handler")));
void ACMP1_Handler(void)             __attribute__((weak, alias("_Unused_Handler")));
void SYSCONTROL_Handler(void)        __attribute__((weak, alias("_Unused_Handler")));
void FLASHCTRL_Handler(void)         __attribute__((weak, alias("_Unused_Handler")));
void GPIOF_Handler(void)             __attribute__((weak, alias("_Unused_Handler")));
void GPIOG_Handler(void)             __attribute__((weak, alias("_Unused_Handler")));
void UART2_Handler(void)             __attribute__((weak, alias("_Unused_Handler")));
void TIMER3A_Handler(void)           __attribute__((weak, alias("_Unused_Handler")));
void TIMER3B_Handler(void)           __attribute__((weak, alias("_Unused_Handler")));
void I2C1_Handler(void)              __attribute__((weak, alias("_Unused_Handler")));
void QEI1_Handler(void)              __attribute__((weak, alias("_Unused_Handler")));
void ETH_Handler(void)               __attribute__((weak, alias("_Unused_Handler")));
void HIB_Handler(void)               __attribute__((weak, alias("_Unused_Handler")));

__attribute__ ((section(".vectors"), used))
void (* const  _exceptions[])(void) = {
    (void (*)(void))((uint32_t)&_sram_stacktop),    //  0: Initial Stack Pointer
    _Reset_Handler,                                 //  1: Reset
    _NMI_Handler,                                   //  2: Non-Maskable Interrupt
    _Hard_Fault_Handler,                            //  3: Hard Fault
    _Memory_Mgmt_Handler,                           //  4: Memory Management Fault
    _Bus_Fault_Handler,                             //  5: Bus Fault
    _Usage_Fault_Handler,                           //  6: Usage Fault
    0,                                              //  7: Reserved
    0,                                              //  8: Reserved
    0,                                              //  9: Reserved
    0,                                              // 10: Reserved
    _SVCall_Handler,                                // 11: Supervisor Call
    _Debug_Monitor_Handler,                         // 12: Debug Monitor
    0,                                              // 13: Reserved
    _PendSV_Handler,                                // 14: PendSV
    _SysTick_Handler,                               // 15: System Tick

    GPIOA_Handler,                                  // 16: GPIO Port A
    GPIOB_Handler,                                  // 17: GPIO Port B
    GPIOC_Handler,                                  // 18: GPIO Port C
    GPIOD_Handler,                                  // 19: GPIO Port D
    GPIOE_Handler,                                  // 20: GPIO Port E
    UART0_Handler,                                  // 21: UART0
    UART1_Handler,                                  // 22: UART1
    SSI0_Handler,                                   // 23: SSI0
    I2C0_Handler,                                   // 24: I2C0
    PWMFAULT_Handler,                               // 25: PWM Fault
    PWMGEN0_Handler,                                // 26: PWM Generator 0
    PWMGEN1_Handler,                                // 27: PWM Generator 1
    PWMGEN2_Handler,                                // 28: PWM Generator 2
    QEI0_Handler,                                   // 29: Quadrature Encoder 0
    ADC0SEQ0_Handler,                               // 30: ADC0 Sequence 0
    ADC0SEQ1_Handler,                               // 31: ADC0 Sequence 1
    ADC0SEQ2_Handler,                               // 32: ADC0 Sequence 2
    ADC0SEQ3_Handler,                               // 33: ADC0 Sequence 3
    WDT_Handler,                                    // 34: Watchdog Timer 0
    TIMER0A_Handler,                                // 35: Timer 0A
    TIMER0B_Handler,                                // 36: Timer 0B
    TIMER1A_Handler,                                // 37: Timer 1A
    TIMER1B_Handler,                                // 38: Timer 1B
    TIMER2A_Handler,                                // 39: Timer 2A
    TIMER2B_Handler,                                // 40: Timer 2B
    ACMP0_Handler,                                  // 41: Analog Comparator 0
    ACMP1_Handler,                                  // 42: Analog Comparator 1
    0,                                              // 43: Reserved
    SYSCONTROL_Handler,                             // 44: System Control
    FLASHCTRL_Handler,                              // 45: Flash Control
    GPIOF_Handler,                                  // 46: GPIO Port F
    GPIOG_Handler,                                  // 47: GPIO Port G
    0,                                              // 48: Reserved
    UART2_Handler,                                  // 49: UART2
    0,                                              // 50: Reserved
    TIMER3A_Handler,                                // 51: Timer 3A
    TIMER3B_Handler,                                // 52: Timer 3B
    I2C1_Handler,                                   // 53: I2C1
    QEI1_Handler,                                   // 54: Quadrature Encoder 1
    0,                                              // 55: Reserved
    0,                                              // 56: Reserved
    0,                                              // 57: Reserved
    ETH_Handler,                                    // 58: Ethernet
    HIB_Handler                                     // 59: Hibernation
};

/* _Reset_Handler is what is invoked when the processor is reset.
 * As seen in the vector table, it represents the initial value of
 * the program counter. This is where we setup and call main()
 * We'll create a separate section .startup so this resides 
 * immediately after the vector table - as required by LM3S6965 (ARM Cortex-M3)
 */
__attribute__ ((section(".startup")))
void _Reset_Handler(void)
{
    /* Let's disable interrupts for now, shall we */
    irq_master_disable();

    /* Copy the data segment from flash to sram */

    uint32_t *pSrc = &_flash_sdata;   
    uint32_t *pDest = &_sram_sdata;

    while(pDest < &_sram_edata)
    {
        *pDest++ = *pSrc++;
    }

    /* Zero initialize the bss segment in sram */
    pDest = &_sram_sbss;
    
    while(pDest < &_sram_ebss)
    {
        *pDest++ = 0;
    }

    /* Call main() */
    main();

   /* main() isn't supposed return 
    * - if it does, we need to identify this
    * for now, we'll loop infintely
    */
    while(1);
}

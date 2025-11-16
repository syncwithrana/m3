#include <stdint.h>
#include "irq.h"
extern uint32_t _flash_sdata;
extern uint32_t _sram_sdata;
extern uint32_t _sram_edata;
extern uint32_t _sram_sbss;
extern uint32_t _sram_ebss;
extern uint32_t _sram_stacktop;
extern void main(void);
extern void uart0_irq_handler(void);
extern void _SysTick_Handler(void);
void _Unused_Handler(void)
{
    while(1);
}
void _Reset_Handler(void);
void _NMI_Handler(void)             __attribute__ ((weak, alias("_Unused_Handler")));
void _Hard_Fault_Handler(void)      __attribute__ ((weak, alias("_Unused_Handler")));
void _Memory_Mgmt_Handler(void)     __attribute__ ((weak, alias("_Unused_Handler")));
void _Bus_Fault_Handler(void)       __attribute__ ((weak, alias("_Unused_Handler")));
void _Usage_Fault_Handler(void)     __attribute__ ((weak, alias("_Unused_Handler")));
void _SVCall_Handler(void)          __attribute__ ((weak, alias("_Unused_Handler")));
void _Debug_Monitor_Handler(void)   __attribute__ ((weak, alias("_Unused_Handler")));
// ...existing code...

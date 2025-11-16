#include <stdint.h>
#include "lm3s6965_memmap.h"
#include "sysctl.h"
#include "systick.h"
#include "uart_drv.h"
#include "serial_print.h"
#define SYS_TIMER_BASE          ((M3_PERIPHERAL_BASE)+ 0x00000010u)
#define MILLISECS_IN_SEC        1000u
#define TWENTY_TICKS            20u
typedef struct __attribute__((packed)){
    uint32_t STCTRL;
    uint32_t STRELOAD;
    uint32_t STCURRENT;
}systick_regs;
static volatile systick_regs *systick = (systick_regs*)SYS_TIMER_BASE;
static volatile uint32_t tick_count = 0;
void systick_enable(void)
{
    systick->STCTRL |= STCTRL_CLKSRC | STCTRL_ENABLE;
}
void systick_disable(void)
{
    systick->STCTRL &= ~(STCTRL_ENABLE);
}
// ...existing code...

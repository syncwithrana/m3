#include <stdint.h>
#include "lm3s6965_memmap.h"
#include "sysctl.h"
#include "systick.h"
#include "uart_drv.h"
#include "serial_print.h"

#define SYS_TIMER_BASE          ((M3_PERIPHERAL_BASE)+ 0x00000010u)
#define MILLISECS_IN_SEC        1000u
#define TWENTY_TICKS            20u

typedef struct __attribute__((packed))  {
    uint32_t STCTRL;            //0x0 SysTick Control and Status Register
    uint32_t STRELOAD;          //0x4 SysTick Reload Value Register
    uint32_t STCURRENT;         //0x8 SysTick Current Value Register
}systick_regs;

static volatile systick_regs *systick = (systick_regs*)SYS_TIMER_BASE;
static volatile uint32_t tick_count = 0;

void systick_enable(void)   {
    systick->STCTRL |= STCTRL_CLKSRC | STCTRL_ENABLE;
}

void systick_disable(void)  {
    systick->STCTRL &= ~(STCTRL_ENABLE);

}

void systick_irq_enable(void)   {
    systick->STCTRL |= STCTRL_INTEN; 
}

void systick_irq_disable(void)  {
    systick->STCTRL &= ~(STCTRL_INTEN);
}

static uint32_t systick_millisec_to_timer_period(uint32_t millisec) {
    uint32_t period = (sysctl_getclk()/1000u) * millisec;
    return period;

}

void systick_set_period_ms(uint32_t millisec)   {
    uint32_t count = systick_millisec_to_timer_period(millisec);
    systick->STRELOAD = count;
}

void _SysTick_Handler(void) {
    tick_count++;
    if(tick_count % 20u == 0)
    {
        serial_put_uint(tick_count);
        serial_puts(" time ticks have elapsed!\n");
    }
}

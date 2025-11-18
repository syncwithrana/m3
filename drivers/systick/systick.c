#include <stdint.h>
#include "lm3s6965_memmap.h"
#include "sysctl.h"
#include "systick.h"
#include "uart_drv.h"
#include "serial_print.h"

/* SysTick timer driver
 *
 * Notes:
 * - SYS_TIMER_BASE is an offset from M3_PERIPHERAL_BASE defined in the
 *   platform memory map. This is machine-dependent and must match the
 *   target MCU memory map in `platform/lm3s6965_memmap.h`.
 * - The SysTick handler does very little work (increments a tick counter
 *   and occasionally prints). Avoid heavy processing in ISRs; use flags
 *   or deferred work if you need more complex behavior.
 */
#define SYS_TIMER_BASE          ((M3_PERIPHERAL_BASE)+ 0x00000010u)
#define MILLISECS_IN_SEC        1000u
#define TWENTY_TICKS            20u

/* Minimal view of SysTick registers (offsets match Cortex-M SysTick). */
typedef struct __attribute__((packed))  {
    uint32_t STCTRL;            //0x0 SysTick Control and Status Register
    uint32_t STRELOAD;          //0x4 SysTick Reload Value Register
    uint32_t STCURRENT;         //0x8 SysTick Current Value Register
} systick_regs;

static volatile systick_regs *systick = (systick_regs*)SYS_TIMER_BASE;
static volatile uint32_t tick_count = 0;

/* Public API: enable the SysTick timer with processor clock source. */
void systick_enable(void)   {
    systick->STCTRL |= STCTRL_CLKSRC | STCTRL_ENABLE;
}

/* Public API: disable SysTick. */
void systick_disable(void)  {
    systick->STCTRL &= ~(STCTRL_ENABLE);
}

/* Public API: enable SysTick exception/interrupt. */
void systick_irq_enable(void)   {
    systick->STCTRL |= STCTRL_INTEN; 
}

/* Public API: disable SysTick exception/interrupt. */
void systick_irq_disable(void)  {
    systick->STCTRL &= ~(STCTRL_INTEN);
}

/* Convert milliseconds to a SysTick reload value.
 *
 * Note: The calculation uses `sysctl_getclk()` which returns the CPU
 * clock in Hz. Careful: (sysclk/1000) * millisec can overflow for large
 * values on 32-bit math. This is intended for reasonable millisecond
 * ranges (e.g., < a few million). If you need very large intervals,
 * consider using a 64-bit intermediate or splitting the timer.
 */
static uint32_t systick_millisec_to_timer_period(uint32_t millisec) {
    uint32_t period = (sysctl_getclk()/1000u) * millisec;
    return period;
}

/* Set the SysTick reload count using milliseconds. */
void systick_set_period_ms(uint32_t millisec)   {
    uint32_t count = systick_millisec_to_timer_period(millisec);
    systick->STRELOAD = count;
}

/* SysTick ISR: increment tick_count and occasionally print a message.
 * Keep ISR work small: printing from an ISR is acceptable for demos but
 * not recommended for production real-time code. */
void _SysTick_Handler(void) {
    tick_count++;
    if(tick_count % 20u == 0)
    {
        serial_put_uint(tick_count);
        serial_puts(" time ticks have elapsed!\n");
    }
}

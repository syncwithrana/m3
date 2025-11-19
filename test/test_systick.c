/* test_systick.c
 *
 * Demonstrates SysTick configuration and periodic tick printing.
 * This test enables interrupts, configures system clock, programs
 * the SysTick reload period (500 ms), and starts the timer. It is
 * intended for QEMU or a development board — it performs blocking
 * prints and does not return.
 */

#include <stdint.h>
#include "irq.h"
#include "nvic.h"
#include "sysctl.h"
#include "timer.h"
#include "comms_drv.h"
#include "console.h"

int main(void)
{
    irq_master_enable();
    irq_enable(IRQ_UART0);
    comms_init(COMMS_BAUD_115200);

    uint32_t clk_cfg1 = (CLK_PLL_SYSCLK | CLK_RCC_USESYSDIV | CLK_RCC_SYSDIV_11 | 
               CLK_RCC_XTAL_8MHZ | CLK_RCC_OSCSRC_MOSC | CLK_RCC_IOSCDIS);
    uint32_t clk_cfg2 = 0;
    
    console_puts("Configuring system clock...: ");
    console_put_uint(clk_cfg1);
    console_putc('\n');

    clock_set_config(clk_cfg1, clk_cfg2);
    timer_set_period_ms(500u);
    timer_irq_enable();
    timer_enable();

    console_puts("System Initialized.\r\n");

    while(1);

    return 0;
}

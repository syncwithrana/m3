#include <stdint.h>
#include "irq.h"
#include "nvic.h"
#include "sysctl.h"
#include "systick.h"
#include "uart_drv.h"
#include "serial_print.h"
int main(void)
{
    irq_master_enable();
    nvic_irq_enable(IRQ_UART0);
    uint32_t clk_cfg1 = (SYSCTL_PLL_SYSCLK | SYSCTL_RCC_USESYSDIV | SYSCTL_RCC_SYSDIV_11 | 
               SYSCTL_RCC_XTAL_8MHZ | SYSCTL_RCC_OSCSRC_MOSC | SYSCTL_RCC_IOSCDIS);
    uint32_t clk_cfg2 = 0;
    serial_puts("Configuring system clock...: ");
    serial_put_uint(clk_cfg1);
    serial_putchar('\n');
    sysctl_setclk(clk_cfg1, clk_cfg2);
    systick_set_period_ms(500u);
    systick_irq_enable();
    systick_enable();
    uart_init(UART_BAUD_115200);
    serial_puts("System Initialized.\r\n");
    while(1);
    return 0;
}

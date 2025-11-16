#include <stdint.h>
#include "irq.h"
#include "nvic.h"
#include "sysctl.h"
#include "systick.h"
#include "uart_drv.h"
#include "serial_print.h"

int main(void)
{
    /* Let's now re-enable the interrupts*/
    irq_master_enable();

    /* Also, let's also turn on the UART0 interrupt */
    nvic_irq_enable(IRQ_UART0);

    /* Configure the uart to a baud-rate of 115200 */
    uart_init(UART_BAUD_115200);

    /* Set the system clock to the PLL with the main oscillator as the source
     * with the crystal frequency set to 8 MHz. 
     * Divide the PLL output clock frquency by a factor of 12.
     * Turn off the (unused) internal oscillator. This is to configure a system clock of 16.67 MHz.
     */
    uint32_t clk_cfg1 = (SYSCTL_PLL_SYSCLK | SYSCTL_RCC_USESYSDIV | SYSCTL_RCC_SYSDIV_11 | 
               SYSCTL_RCC_XTAL_8MHZ | SYSCTL_RCC_OSCSRC_MOSC | SYSCTL_RCC_IOSCDIS);
    uint32_t clk_cfg2 = 0;
    
    serial_puts("Configuring system clock...: ");
    serial_put_uint(clk_cfg1);
    serial_putchar('\n');

    sysctl_setclk(clk_cfg1, clk_cfg2);
    
    /* Let's set systick period to be 0.5 seconds =>
     * system clock frequency divided by 2.
     */
    systick_set_period_ms(500u);

    /* Let's enable the systick timer and it's interrupt */
    systick_irq_enable();
    systick_enable();


    serial_puts("System Initialized.\r\n");

    while(1);

    return 0;
}

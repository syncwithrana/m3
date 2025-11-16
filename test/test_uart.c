#include "irq.h"
#include "nvic.h"
#include "uart_drv.h"
#include "serial_print.h"

int main(void)
{
    char first_ip[64];

    /* Let's now re-enable the interrupts*/
    irq_master_enable();

    // /* Also, let's also turn on the UART0 interrupt */
    // nvic_irq_enable(IRQ_UART0);

    /* Configure the uart to a baud-rate of 115200 */
    uart_init(115200);

    serial_puts("System Initialized\r\n");
    serial_putchar('.');
    serial_putchar('.');
    serial_putchar('.');
    serial_puts("\nGo on, say something...\n");

    while(1)
    {
        serial_gets(first_ip);

        serial_puts("You said:\t");
        serial_puts(first_ip);
        serial_putchar('\n');
    }

    return 0;
}
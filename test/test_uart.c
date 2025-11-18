#include "irq.h"
#include "nvic.h"
#include "uart_drv.h"
#include "serial_print.h"

int main(void)
{
    char first_ip[64];
    irq_master_enable();
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
#include "uart_drv.h"
#include "serial_print.h"

int main(void)
{
    const char *start_msg = "Hello, World\n";
    char first_ip[64];

    uart_init(115200);

    serial_puts(start_msg);

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
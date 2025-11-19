/* test_comms.c
 *
 * Simple COMMS echo test (previously test_uart.c). Initializes the COMMS
 * peripheral and echoes lines typed on the console.
 */

#include "irq.h"
#include "nvic.h"
#include "comms_drv.h"
#include "console.h"

int main(void)
{
    char first_ip[64];
    irq_master_enable();
    comms_init(115200);

    console_puts("System Initialized\r\n");
    console_putc('.');
    console_putc('.');
    console_putc('.');
    console_puts("\nGo on, say something...\n");

    while(1)
    {
        console_readline(first_ip);
        console_puts("You said:\t");
        console_puts(first_ip);
        console_putc('\n');
    }

    return 0;
}

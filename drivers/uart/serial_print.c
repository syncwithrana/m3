#include <stdint.h>
#include "uart_drv.h"
#include "serial_print.h"

/* Simple serial print helpers built on top of the UART driver.
 *
 * These are small convenience functions used by test programs. They are
 * intentionally minimal: blocking I/O, no buffering, and no bounds checks
 * for `serial_gets`. For production use, consider adding timeouts and
 * input buffer length checks to avoid overruns.
 */
void serial_putchar(const char c)   {
    /* Blocking write of a single character. */
    uart_tx_byte(c);
}

/* Blocking read of a single character. Returns UART_OK or UART_NO_DATA.
 * This simply wraps the lower-level uart_rx_byte function. */
uart_err serial_getchar(char* pc)   {
    uart_err retval;
    retval = uart_rx_byte((uint8_t*)pc);
    return retval;
}

/* Write a NUL-terminated string to the serial port (blocking). */
void serial_puts(const char* str)   {
    while(*str) {
        serial_putchar(*str);
        str++;
    }
}

/* Read a line (CR-terminated) into the provided buffer.
 *
 * WARNING: no length is provided; callers must ensure `str` points to a
 * sufficiently large buffer. This function blocks until a CR is received.
 */
void serial_gets(char* str) {
    char c;
    char *pstr = str;

    while(1)  {
        if(serial_getchar(&c) == UART_OK) {
            if(c == '\r'){
                *pstr = '\0';
                break;
            }
            else{
                *pstr = c;
            }
            
            pstr++;
        }
    }
}

/* Convert unsigned integer to ASCII and print. */
void serial_put_uint(uint32_t num)  {
    char num_buf[10] = {0};
    uint8_t idx = 0;
    uint8_t remainder;

    do {
        remainder = num % 10;
        num_buf[idx] = '0' + remainder;
        idx++;
        num /= 10;
    }while(num != 0);

    while(idx)  {
        serial_putchar(num_buf[idx-1]);
        idx--;
    }
}

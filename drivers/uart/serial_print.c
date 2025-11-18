#include <stdint.h>
#include "uart_drv.h"
#include "serial_print.h"

void serial_putchar(const char c)   {
    uart_tx_byte(c);
}

uart_err serial_getchar(char* pc)   {
    uart_err retval;
    retval = uart_rx_byte(pc);
    return retval;
}

void serial_puts(const char* str)   {
    while(*str) {
        serial_putchar(*str);
        str++;
    }
}

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

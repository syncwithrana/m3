#include <stdint.h>
#include "comms_drv.h"
#include "console.h"

void console_putc(const char c)   {
    comms_send_byte(c);
}

comms_status console_getc(char* pc)   {
    comms_status retval;
    retval = comms_recv_byte((uint8_t*)pc);
    return retval;
}

void console_puts(const char* str)   {
    while(*str) {
        console_putc(*str);
        str++;
    }
}

void console_readline(char* str) {
    char c;
    char *pstr = str;

    while(1)  {
        if(console_getc(&c) == COMMS_OK) {
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

void console_put_uint(uint32_t num)  {
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
        console_putc(num_buf[idx-1]);
        idx--;
    }
}

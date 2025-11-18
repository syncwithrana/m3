/* serial_print.h
 *
 * Tiny convenience wrappers around the UART driver for simple I/O in
 * test applications. Functions are blocking and minimal; callers must
 * ensure buffers are large enough for `serial_gets`.
 */

#ifndef __SERIAL_PRINT_H__
#define __SERIAL_PRINT_H__

void serial_putchar(const char c);
uart_err serial_getchar(char* pc);
void serial_puts(const char* str);
void serial_gets(char* str);
void serial_put_uint(uint32_t num);

#endif /*  __SERIAL_PRINT_H__ */

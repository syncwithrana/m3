/* console.h
 *
 * Tiny convenience wrappers around the COMMS driver for simple I/O in
 * test applications. Functions are blocking and minimal; callers must
 * ensure buffers are large enough for `console_readline`.
 */

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

void console_putc(const char c);
comms_status console_getc(char* pc);
void console_puts(const char* str);
void console_readline(char* str);
void console_put_uint(uint32_t num);

#endif /*  __CONSOLE_H__ */

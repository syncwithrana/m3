#ifndef __UART_DRV_H__
#define __UART_DRV_H__

typedef enum{
    UART_OK = 0,
    UART_NO_DATA,
    UART_RX_ERR,
}uart_err;

/* Configured as in the example 
 * Refer http://www.ti.com/lit/ds/symlink/lm3s6965.pdf 12.4
 * and http://www.ti.com/lit/ds/symlink/lm3s6965.pdf for UARTLCRH description.
 */
#define UARTLCRH_EXAMPLE    0x00000060u
#define UARTLCRH_FEN        0x00000010

#define UARTCTL_UARTEN      0x00000001u

#define UARTFR_TXFF         0x00000020u
#define UARTFR_RXFE         0x00000010u
#endif /* __UART_DRV_H__ */

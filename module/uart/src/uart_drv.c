#include <stdint.h>
#include <stdbool.h>
#include "lm3s6965_memmap.h"
#include "uart_drv.h"
#include "sysctl.h"

/* UART register map structure.
 * Refer: http://www.ti.com/lit/ds/symlink/lm3s6965.pdf Table 12-3.
 */
typedef struct __attribute__ ((packed)){
    uint32_t DR;                // 0x00 UART Data Register
    uint32_t RSRECR;            // 0x04 UART Recieve Status/Error Clear Register
    uint32_t reserved0[4];      // 0x08-0x14 reserved
    const uint32_t FR;          // 0x18 UART Flag Register
    uint32_t reserved1;         // 0x1C reserved
    uint32_t ILPR;              // 0x20 UART IrDA Low-Power Register Register
    uint32_t IBRD;              // 0x24 UART Integer Baud-Rate Divisor Register
    uint32_t FBRD;              // 0x28 UART Fractional Baud-Rate Divisor Register
    uint32_t LCRH;              // 0x2C UART Line Control Register
    uint32_t CTL;               // 0x30 UART Control Register
    uint32_t IFLS;              // 0x34 UART Interrupt FIFO Level Select Register
    uint32_t IM;                // 0x38 UART Interrupt Mask Register
    const uint32_t RIS;         // 0x3C UART Raw Interrupt Status Register
    const uint32_t MIS;         // 0x40 UART Masked Interrupt Status Register
    uint32_t ICR;               // 0x44 UART Interrupt Clear Register
}uart_regs;

/* The MISRA-C:2004, Rule 10.6 
 * Also, Refer: http://www.ti.com/lit/ds/symlink/lm3s6965.pdf Table 2-4.
 * UART0 is one of 3 UART interfaces on LM3S6965 mapped to the adderss shown below.
 */
static volatile uart_regs *uart0 = (uart_regs*)UART0_BASE;

/* Enable the uart
 * TXE and RXE - transmit and recieve enable bits
 * are enabled out of reset - hence we don't set them here.
 */
static void uart_enable(void)
{
    /* set UARTEN in UARTCTL */
    // ...existing code...
}

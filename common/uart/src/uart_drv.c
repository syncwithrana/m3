#include "uart_drv.h"
#include <stdint.h>
#include <stdbool.h>
#include "lm3s6965_memmap.h"

// UART0 base address (can be overridden as needed)
#ifndef UART0_BASE
#define UART0_BASE 0x4000C000u
#endif

static volatile uart_regs *uart0 = (uart_regs*)UART0_BASE;

#define UART_DFLT_SYSCLK 12500000u

static void set_clk_uart0(void)
{
    uint32_t *pRCGC1 = (uint32_t*)0x400FE104u;
    *pRCGC1 |= 0x00000001u;
}

static void uart_enable(void)
{
    uart0->CTL |= UARTCTL_UARTEN;
}

static void uart_disable(void)
{
    uart0->CTL &= ~UARTCTL_UARTEN;
    while(uart0->FR & UARTFR_BUSY);
    uart0->LCRH &= ~UARTLCRH_FEN;
}

static void uart_set_baudrate(uint32_t baudrate)
{
    uint32_t brdi, brdf, dvsr, remd;
    dvsr = (baudrate * 16u);
    brdi = UART_DFLT_SYSCLK/dvsr;
    remd = UART_DFLT_SYSCLK - dvsr * brdi;
    brdf = ((remd << 6u) + (dvsr >> 1))/dvsr;
    uart0->IBRD = (uint16_t)(brdi & 0xffffu);
    uart0->FBRD = (uint8_t)(brdf & 0x3ffu);
}

static void uart_set_example_line_ctrls(void)
{
    uart0->LCRH = UARTLCRH_EXAMPLE;
}

void uart_init(uint32_t baudrate)
{
    uart_disable();
    set_clk_uart0();
    uart_set_baudrate(baudrate);
    uart_set_example_line_ctrls();
    uart_enable();
}

void uart_tx_byte(uint8_t byte)
{
    while(uart0->FR & UARTFR_TXFF);
    uart0->DR = byte & UARTDR_DATA_MASK;
}

uart_err uart_rx_byte(uint8_t* byte)
{
    if(uart0->FR & UARTFR_RXFE)
        return UART_NO_DATA;
    uint32_t data = uart0->DR;
    uint32_t err = uart0->RSRECR & UARTRSRECR_ERR_MASK;
    if(err)
        return UART_RX_ERR;
    *byte = (uint8_t)(data & UARTDR_DATA_MASK);
    return UART_OK;
}

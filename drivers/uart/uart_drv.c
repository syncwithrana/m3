#include <stdint.h>
#include <stdbool.h>
#include "lm3s6965_memmap.h"
#include "uart_drv.h"
#include "sysctl.h"

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

static volatile uart_regs *uart0 = (uart_regs*)UART0_BASE;

static void uart_enable(void)   {
    uart0->CTL |= UARTCTL_UARTEN;
}


static void uart_disable(void)  {
    uart0->CTL &= ~UARTCTL_UARTEN;
    while(uart0->FR & UARTFR_BUSY);
    uart0->LCRH &= ~UARTLCRH_FEN;
}

static void uart_set_baudrate(uint32_t baudrate)    {
    uint32_t sysclk, brdi, brdf, dvsr, remd;
    sysclk = sysctl_getclk();
    dvsr = (baudrate * 16u);

    brdi = sysclk/dvsr;

    remd = sysclk - dvsr * brdi;
    brdf = ((remd << 6u) + (dvsr >> 1))/dvsr;

    uart0->IBRD = (uint16_t)(brdi & 0xffffu);
    uart0->FBRD = (uint8_t)(brdf & 0x3ffu);
}

static void uart_set_example_line_ctrls(void)   {
    uart0->LCRH = UARTLCRH_EXAMPLE;
}

static void uart_irq_enable(uint32_t irq_flags)   {
    uart0->IM |= irq_flags;
}

static void uart_irq_clear(uint32_t irq_flags)  {
    uart0->ICR = irq_flags;
}

static uint32_t uart_irq_status(bool is_masked)  {
    if(is_masked)
        return (uart0->MIS);
    else
        return (uart0->RIS);
}

void uart_init(uint32_t baudrate)   {
    uart_disable();
    sysctl_periph_clk_enable(UART0_BASE);
    uart_set_baudrate(baudrate);
    uart_set_example_line_ctrls();
    uart_enable();
}

void uart_tx_byte(uint8_t byte) {
    while(uart0->FR & UARTFR_TXFF);  
    uart0->DR = (uint32_t)byte;
}

uart_err uart_rx_byte(uint8_t* byte)  {
    if(uart0->FR & UARTFR_RXFE) {
        return UART_NO_DATA;
    }

    *byte = uart0->DR & UARTDR_DATA_MASK;
    
    if(uart0->RSRECR & UARTRSRECR_ERR_MASK) {
        uart0->RSRECR &= UARTRSRECR_ERR_MASK;
        return UART_RX_ERR;
    }
    
    return UART_OK;
}

void uart0_irq_handler(void)    {
    uint32_t irq_status;
    char c;

    irq_status = uart_irq_status(true);

    uart_irq_clear(irq_status);

    if(irq_status & UART_RX_IRQ) {
        c = uart0->DR & UARTDR_DATA_MASK;
        
        if(c == '\r') {
            uart_tx_byte('\n');
        }
        else if(c == '\b')  {
            uart_tx_byte('X');
        }
        else  {
            uart_tx_byte(c);
        }
    }
}

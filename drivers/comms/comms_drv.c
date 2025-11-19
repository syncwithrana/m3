#include <stdint.h>
#include <stdbool.h>
#include "lm3s6965_memmap.h"
#include "comms_drv.h"
#include "sysctl.h"

typedef struct __attribute__ ((packed)){
    uint32_t DR;                // 0x00 UART Data Register
    uint32_t RSRECR;            // 0x04 UART Receive Status/Error Clear Register
    uint32_t reserved0[4];      // 0x08-0x14 reserved
    const uint32_t FR;          // 0x18 UART Flag Register (read-only)
    uint32_t reserved1;         // 0x1C reserved
    uint32_t ILPR;              // 0x20 UART IrDA Low-Power Register
    uint32_t IBRD;              // 0x24 UART Integer Baud-Rate Divisor Register
    uint32_t FBRD;              // 0x28 UART Fractional Baud-Rate Divisor Register
    uint32_t LCRH;              // 0x2C UART Line Control Register
    uint32_t CTL;               // 0x30 UART Control Register
    uint32_t IFLS;              // 0x34 UART Interrupt FIFO Level Select Register
    uint32_t IM;                // 0x38 UART Interrupt Mask Register
    const uint32_t RIS;         // 0x3C UART Raw Interrupt Status Register (read-only)
    const uint32_t MIS;         // 0x40 UART Masked Interrupt Status Register (read-only)
    uint32_t ICR;               // 0x44 UART Interrupt Clear Register
} uart_regs;

typedef uart_regs comms_regs;

static volatile comms_regs *comms0 = (comms_regs*)UART0_BASE;

static void comms_enable(void)   {
    comms0->CTL |= COMMSCTL_UARTEN;
}

static void comms_disable(void)  {
    comms0->CTL &= ~COMMSCTL_UARTEN;
    while(comms0->FR & COMMSFR_BUSY);
    comms0->LCRH &= ~COMMSLCRH_FEN;
}

static void comms_set_baudrate(uint32_t baudrate)    {
    uint32_t sysclk, brdi, brdf, dvsr, remd;
    sysclk = clock_get_hz();
    dvsr = (baudrate * 16u);

    brdi = sysclk/dvsr;

    remd = sysclk - dvsr * brdi;
    brdf = ((remd << 6u) + (dvsr >> 1))/dvsr;

    comms0->IBRD = (uint16_t)(brdi & 0xffffu);
    comms0->FBRD = (uint32_t)(brdf & 0x3ffu);
}

static void comms_set_example_line_ctrls(void)   {
    comms0->LCRH = COMMSLCRH_EXAMPLE;
}

static void comms_irq_enable(uint32_t irq_flags)   {
    comms0->IM |= irq_flags;
}

static void comms_irq_clear(uint32_t irq_flags)  {
    comms0->ICR = irq_flags;
}

static uint32_t comms_irq_status(bool is_masked)  {
    if(is_masked)
        return (comms0->MIS);
    else
        return (comms0->RIS);
}

void comms_init(uint32_t baudrate)   {
    comms_disable();
    clock_periph_enable(UART0_BASE);
    comms_set_baudrate(baudrate);
    comms_set_example_line_ctrls();
    comms_enable();
}

void comms_send_byte(uint8_t byte) {
    while(comms0->FR & COMMSFR_TXFF);
    comms0->DR = (uint32_t)byte;
}

comms_status comms_recv_byte(uint8_t* byte)  {
    if(comms0->FR & COMMSFR_RXFE) {
        return COMMS_NO_DATA;
    }

    *byte = comms0->DR & COMMSDR_DATA_MASK;
    
    if(comms0->RSRECR & COMMSRSRECR_ERR_MASK) {
        comms0->RSRECR &= COMMSRSRECR_ERR_MASK;
        return COMMS_RX_ERR;
    }
    
    return COMMS_OK;
}

void comms0_irq_handler(void)    {
    uint32_t irq_status;
    char c;

    irq_status = comms_irq_status(true);

    comms_irq_clear(irq_status);

    if(irq_status & COMMS_RX_IRQ) {
        c = comms0->DR & COMMSDR_DATA_MASK;
        
        if(c == '\r') {
            comms_send_byte('\n');
        }
        else if(c == '\b')  {
            comms_send_byte('X');
        }
        else  {
            comms_send_byte(c);
        }
    }
}

void UART0_Handler(void) {
    comms0_irq_handler();
}

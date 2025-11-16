#include <stdint.h>
#include "lm3s6965_memmap.h"
#include "nvic.h"
#define NVIC_BASE       ((M3_PERIPHERAL_BASE)+ 0x00000100u)
typedef struct __attribute__ ((packed)){
    uint32_t EN0;
    uint32_t EN1;
    uint32_t reserved[30];
    uint32_t DIS0;
    uint32_t DIS1;
    uint32_t reserved1[30];
    uint32_t PEND0;
    uint32_t PEND1;
    uint32_t reserved2[30];
    uint32_t UNPEND0;
    uint32_t UNPEND1;
    uint32_t reserved3[30];
    uint32_t ACTIVE0;
    uint32_t ACTIVE1;
    uint32_t reserved4[62];
    uint32_t PRI0;
    uint32_t PRI1;
    uint32_t PRI2;
    uint32_t PRI3;
    uint32_t PRI4;
    uint32_t PRI5;
    uint32_t PRI6;
    uint32_t PRI7;
    uint32_t PRI8;
    uint32_t PRI9;
    uint32_t PRI10;
}nvic_regs;
static volatile nvic_regs *nvic = (nvic_regs*)NVIC_BASE;
// ...existing code...

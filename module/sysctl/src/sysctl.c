#include <stdint.h>
#include "lm3s6965_memmap.h"
#include "sysctl.h"
#define FREQ_12MHZ 12000000u
#define FREQ_30KHZ 30000u
#define FREQ_32KHZ 32000u
typedef struct __attribute__((packed)){
    uint32_t DID0;
    uint32_t DID1;
    uint32_t DC0;
    uint32_t reserved1;
    uint32_t DC1;
    uint32_t DC2;
    uint32_t DC3;
    uint32_t DC4;
    uint32_t reserved2[4];
    uint32_t PBORCTL;
    uint32_t LDOPCTL;
    uint32_t reserved3[2];
    uint32_t SRCR0;
    uint32_t SRCR1;
    uint32_t SRCR2;
    uint32_t reserved4;
    uint32_t RIS;
    uint32_t IMC;
    uint32_t MISC;
    uint32_t RESC;
    uint32_t RCC;
    uint32_t PLLCFG;
    uint32_t reserved5[2];
    uint32_t RCC2;
    uint32_t reserved6[35];
    uint32_t RCGC0;
    uint32_t RCGC1;
    uint32_t RCGC2;
}sysctl_regs;
// ...existing code...

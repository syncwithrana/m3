#include <stdint.h>
#include "lm3s6965_memmap.h"
#include "sysctl.h"

#define FREQ_12MHZ 12000000u
#define FREQ_30KHZ 30000u
#define FREQ_32KHZ 32000u

/* System control register block for LM3S6965.
 *
 * Machine-dependent notes:
 * - The offsets and register meanings come directly from the MCU
 *   datasheet. If you port this code to a different MCU family, the
 *   base address and the register layout must be updated accordingly.
 * - The struct uses `packed` to avoid unexpected padding that would
 *   shift register offsets.
 */
typedef struct __attribute__((packed)){
    uint32_t DID0;              // 0x000 Device Identification 0
    uint32_t DID1;              // 0x004 Device Identification 1
    uint32_t DC0;               // 0x008 Device Capabilities 0
    uint32_t reserved1;         // 0x00C-0x00F reserved
    uint32_t DC1;               // 0x010 Device Capabilities 1
    uint32_t DC2;               // 0x014 Device Capabilities 2
    uint32_t DC3;               // 0x018 Device Capabilities 3
    uint32_t DC4;               // 0x01C Device Capabilities 4
    uint32_t reserved2[4];      // 0x020-0x02F reserved
    uint32_t PBORCTL;           // 0x030 Brown-Out Reset Control
    uint32_t LDOPCTL;           // 0x034 LDO Power Control
    uint32_t reserved3[2];      // 0x038-0x3F reserved
    uint32_t SRCR0;             // 0x040 Software Reset Control 0
    uint32_t SRCR1;             // 0x044 Software Reset Control 1
    uint32_t SRCR2;             // 0x048 Software Reset Control 2
    uint32_t reserved4;         // 0x04C-0x04F reserved
    uint32_t RIS;               // 0x050 Raw Interrupt Status
    uint32_t IMC;               // 0x054 Interrupt Mask Control
    uint32_t MISC;              // 0x058 Masked Interrupt Status and Clear
    uint32_t RESC;              // 0x05C Reset Cause
    uint32_t RCC;               // 0x060 Run-Mode Clock Configuration
    uint32_t PLLCFG;            // 0x064 XTAL to PLL Translation
    uint32_t reserved5[2];      // 0x068-0x06F reserved
    uint32_t RCC2;              // 0x070 Run-Mode Clock Configuration 2
    uint32_t reserved6[35];     // 0x074-0x0FF reserved
    uint32_t RCGC0;             // 0x100 Run Mode Clock Gating Control Register 0
    uint32_t RCGC1;             // 0x104 Run Mode Clock Gating Control Register 1
    uint32_t RCGC2;             // 0x108 Run Mode Clock Gating Control Register 2
    uint32_t reserved7;         // 0x10C-0x10F reserved
    uint32_t SCGC0;             // 0x110 Sleep Mode Clock Gating Control Register 0
    uint32_t SCGC1;             // 0x114 Sleep Mode Clock Gating Control Register 1
    uint32_t SCGC2;             // 0x118 Sleep Mode Clock Gating Control Register 2
    uint32_t reserved8;         // 0x11C-0x11F reserved
    uint32_t DCGC0;             // 0x120 Deep Sleep Mode Clock Gating Control Register 0
    uint32_t DCGC1;             // 0x124 Deep Sleep Mode Clock Gating Control Register 1
    uint32_t DCGC2;             // 0x128 Deep Sleep Mode Clock Gating Control Register 2
    uint32_t reserved9[6];      // 0x12C-0x140 reserved
    uint32_t DSLPCLKCFG;         // 0x144 Deep Sleep Clock Configuration
} sysctl_regs;

typedef sysctl_regs clock_regs;

static volatile clock_regs *clock = (clock_regs*)SYS_CTL_BASE;

static const uint32_t xtal_freq[] = 
{
    1000000,
    1843200,
    2000000,
    2457600,
    3579545,
    3686400,
    4000000,
    4096000,
    4915200,
    5000000,
    5120000,
    6000000,
    6144000,
    7372800,
    8000000,
    8192000    
};

/*
 * Wait for the PLL to indicate lock via the Raw Interrupt Status register.
 *
 * Notes:
 * - The implementation polls the `RIS` register for the PLL lock flag.
 * - A timeout counter is used to avoid infinite loops in case the PLL
 *   never locks. The `CLK_PLLLOCK_DELAY` symbol controls the timeout.
 * - There is an unusual assignment in the original loop condition; the
 *   intent is to loop until `wait` reaches zero or the PLL locks. We
 *   keep the polling behavior but document the expectation here. If the
 *   timeout behavior needs to be adjusted, change the `CLK_PLLLOCK_DELAY`
 *   constant in the header.
 */
static inline void clock_wait_pll_lock(void)
{
    uint32_t wait = CLK_PLLLOCK_DELAY;

    while (wait)
    {
        if (clock->RIS & CLK_RIS_PLLRIS)
        {
            return;
        }

        wait--;
    }
}

/*
 * Simple cycle-counting delay implemented in naked assembly.
 *
 * Usage notes:
 * - This busy-wait reduces high-level jitter but is sensitive to
 *   compiler/optimization changes which is why it is in assembly.
 * - The delay time in real seconds depends on CPU clock rate; this
 *   helper is intended for short delays during clock switching where
 *   precise timing is not required.
 */
static void __attribute__((naked)) clock_delay(uint32_t count)
{
    __asm__("subs r0, #1;"
            "bne clock_delay;"
            "bx lr;"
           );
}

void clock_set_config(uint32_t cfg_rcc, uint32_t cfg_rcc2)
{
    uint32_t tmp_rcc, tmp_rcc2;
    uint32_t osc_delay = CLK_FAST_OSCDELAY;

    /*
     * Configure system clock selection and PLL settings.
     *
     * This routine programs the RCC/RCC2 registers according to the
     * requested config values, toggling bypass and power-down bits as
     * required. The sequence follows the hardware recommended steps to
     * safely switch oscillator sources and engage the PLL.
     *
     * Important (machine-dependent): The exact meaning of bits in
     * RCC/RCC2 (XTAL, OSCSRC, SYSDIV, BYPASS, etc.) are device-specific.
     * Verify `cfg_rcc`/`cfg_rcc2` are constructed using constants from
     * `include/sysctl.h` that match your target MCU.
     */

    tmp_rcc = clock->RCC;
    tmp_rcc2 = clock->RCC2;
    
    tmp_rcc |= CLK_RCC_BYPASS;
    tmp_rcc &= ~(CLK_RCC_USESYSDIV);
    tmp_rcc2 |= CLK_RCC2_BYPASS2;

    clock->RCC = tmp_rcc;
    clock->RCC2 = tmp_rcc2;

     if(((tmp_rcc & CLK_RCC_IOSCDIS) != 0 && (cfg_rcc & CLK_RCC_IOSCDIS) == 0) ||
         ((tmp_rcc & CLK_RCC_MOSCDIS) != 0 && (cfg_rcc & CLK_RCC_MOSCDIS) == 0))
    {
        tmp_rcc &= (~(CLK_RCC_IOSCDIS | CLK_RCC_MOSCDIS) |
                (cfg_rcc & (CLK_RCC_IOSCDIS | CLK_RCC_MOSCDIS)));

        clock->RCC = tmp_rcc;

        if((tmp_rcc2 & CLK_RCC2_USERCC2))
        {
                if(((tmp_rcc2 & CLK_RCC2_OSCSRC2_MASK) == CLK_RCC2_OSCSRC2_30KHZ) ||
                    ((tmp_rcc2 & CLK_RCC2_OSCSRC2_MASK) == CLK_RCC2_OSCSRC2_32KHZ))
            {
                osc_delay = CLK_SLOW_OSCDELAY;
            }
        }
        else
        {
            if((tmp_rcc & CLK_RCC_OSCSRC_MASK) == CLK_RCC_OSCSRC_30KHZ) 
            {
                osc_delay = CLK_SLOW_OSCDELAY;
            }
           
        }

        clock_delay(osc_delay);
    }

    tmp_rcc &= ~(CLK_RCC_XTAL_MASK| CLK_RCC_OSCSRC_MASK);
    tmp_rcc |= cfg_rcc & (CLK_RCC_XTAL_MASK | CLK_RCC_OSCSRC_MASK);

    tmp_rcc2 &= ~(CLK_RCC2_USERCC2 | CLK_RCC2_OSCSRC2_MASK);
    tmp_rcc2 |= cfg_rcc2 & (CLK_RCC2_USERCC2 | CLK_RCC2_OSCSRC2_MASK);

    tmp_rcc &= ~(CLK_RCC_PWRDN);
    tmp_rcc |= cfg_rcc & CLK_RCC_PWRDN;

    tmp_rcc2 &= ~(CLK_RCC2_PWRDN2);
    tmp_rcc2 |= cfg_rcc2 & CLK_RCC2_PWRDN2;

    clock->MISC |= CLK_MISC_PLLIM;
    
    clock->RCC = tmp_rcc;
    clock->RCC2 = tmp_rcc2;

    clock_delay(16);

    tmp_rcc &= ~(CLK_RCC_USESYSDIV | CLK_RCC_SYSDIV_MASK |
                  CLK_RCC_IOSCDIS | CLK_RCC_IOSCDIS);
    tmp_rcc |= cfg_rcc & (CLK_RCC_USESYSDIV | CLK_RCC_SYSDIV_MASK |
                  CLK_RCC_IOSCDIS | CLK_RCC_IOSCDIS);

    tmp_rcc2 &= ~(CLK_RCC2_SYSDIV2_MASK);
    tmp_rcc2 |= cfg_rcc2 & (CLK_RCC2_SYSDIV2_MASK);

    clock->RCC = tmp_rcc;
    clock->RCC2 = tmp_rcc2;

    if((cfg_rcc & CLK_RCC_BYPASS) == 0)
    {
        clock_wait_pll_lock();

        tmp_rcc  &= ~(CLK_RCC_BYPASS); 
        tmp_rcc2 &= ~(CLK_RCC2_BYPASS2);
    }

    clock->RCC = tmp_rcc;
    clock->RCC2 = tmp_rcc2;

    clock_delay(16);
                    
}

uint32_t clock_get_hz(void)
{
    uint32_t tmp_rcc, tmp_rcc2, tmp_pllcfg, clk_rt;

    /*
     * Return the current system clock frequency in Hz.
     *
     * This reads RCC/RCC2 and PLLCFG to calculate the effective CPU
     * clock, taking into account oscillator source, PLL multiplication,
     * and system dividers. The calculation mirrors the logic in the
     * MCU reference manual.
     */

    tmp_rcc = clock->RCC;
    tmp_rcc2 = clock->RCC2;

        switch((tmp_rcc2 & CLK_RCC2_USERCC2) ?
            (tmp_rcc2 & CLK_RCC2_OSCSRC2_MASK) :
            (tmp_rcc & CLK_RCC_OSCSRC_MASK))
    {
        case CLK_RCC_OSCSRC_MOSC:
        {
            clk_rt = xtal_freq[(tmp_rcc & CLK_RCC_XTAL_MASK) >> CLK_RCC_XTAL_SHIFT];
            break;
        }

        case CLK_RCC_OSCSRC_IOSC:
        {
            clk_rt = FREQ_12MHZ;
            break;
        }
        case CLK_RCC_OSCSRC_IOSC_DIV4:
        {
            clk_rt = (FREQ_12MHZ)/4;
            break;
        }
        case CLK_RCC_OSCSRC_30KHZ:
        {
            clk_rt = FREQ_30KHZ;
            break;
        }
        case CLK_RCC2_OSCSRC2_32KHZ:
        {
            clk_rt = FREQ_32KHZ;
            break;
        }

    }

     if(((tmp_rcc2 & CLK_RCC2_USERCC2) != 0 && (tmp_rcc2 & CLK_RCC2_BYPASS2) == 0) ||
         ((tmp_rcc2 & CLK_RCC2_USERCC2) == 0 && (tmp_rcc & CLK_RCC_BYPASS) == 0))
     {
          tmp_pllcfg = clock->PLLCFG;
        
          clk_rt = clk_rt * 
                     ((tmp_pllcfg & CLK_PLLCFG_FVAL_MASK) >> CLK_PLLCFG_FVAL_SHIFT) / 
                     (((tmp_pllcfg & CLK_PLLCFG_RVAL_MASK) >> CLK_PLLCFG_RVAL_SHIFT) + 1);
     }

    if(tmp_rcc & CLK_RCC_USESYSDIV)
    {
        if((tmp_rcc2 & CLK_RCC2_USERCC2))
        {
            if((tmp_rcc2 & CLK_RCC2_BYPASS2) == 0)
            {
                clk_rt = clk_rt/2;
            }

             clk_rt = clk_rt / 
                 (((tmp_rcc2 & CLK_RCC2_SYSDIV2_MASK) >> 
                  CLK_RCC2_SYSDIV2_SHIFT) + 1);
        }
        else 
        {
            if((tmp_rcc & CLK_RCC_BYPASS) == 0)
            {
                clk_rt = clk_rt/2;
            }

            clk_rt = clk_rt / 
                     (((tmp_rcc & CLK_RCC_SYSDIV_MASK) >> 
                      CLK_RCC_SYSDIV_SHIFT) + 1);
        }
    }

    return (clk_rt);
}

void clock_periph_enable(uint32_t periph)
{
    /*
     * Enable run-mode clock gating for a peripheral.
     *
     * Parameter `periph` is expected to be the peripheral base address
     * constant (e.g., UART0_BASE). This function maps base-address
     * identifiers to the corresponding RCGC bit(s). Only a small set
     * of peripherals are supported here; add new cases if you enable
     * more peripherals in other drivers.
     */
    switch(periph)
    {
        case UART0_BASE:
            clock->RCGC1 |= CLK_RCGC1_UART0;
            break;
        case UART1_BASE:
            clock->RCGC1 |= CLK_RCGC1_UART1;
            break;
        case UART2_BASE:
            clock->RCGC1 |= CLK_RCGC1_UART2;
            break;
        default:
            break;
    }
}

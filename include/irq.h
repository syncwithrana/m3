/* irq.h
 *
 * Tiny helpers for globally enabling/disabling interrupts on Cortex-M
 * using PRIMASK (CPSIE/CPSID). These operations are machine/architecture
 * dependent (ARM Cortex-M instructions). They are intended for simple
 * demo programs; in production code you may want more structured
 * interrupt masking (save/restore of PRIMASK) to avoid accidental
 * imbalance between enable/disable.
 */

#ifndef __IRQ_H__
#define __IRQ_H__

static inline void irq_master_enable(void)      {
    __asm__ __volatile__ ("cpsie i");
}

static inline void irq_master_disable(void)     {
    __asm__ __volatile__ ("cpsid i");
}

#endif

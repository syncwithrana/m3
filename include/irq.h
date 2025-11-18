#ifndef __IRQ_H__
#define __IRQ_H__

static inline void irq_master_enable(void)      {
    __asm__ __volatile__ ("cpsie i");
}

static inline void irq_master_disable(void)     {
    __asm__ __volatile__ ("cpsid i");
}

#endif

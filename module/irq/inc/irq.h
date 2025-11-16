#ifndef __IRQ_H__
#define __IRQ_H__
/* To enable all interrupts with programmable priority.
 * Refer: Refer http://www.ti.com/lit/ds/symlink/lm3s6965.pdf
 * Table 2-13 and Section 2-3-4
 */
static inline void irq_master_enable(void)
{
    __asm__ __volatile__ ("cpsie i");
}
static inline void irq_master_disable(void)
{
    __asm__ __volatile__ ("cpsid i");
}
#endif /* __IRQ_H__ */

/* timer.h
 *
 * Timer helper definitions and API (renamed from systick.h). These
 * constants (control bits, reload masks) match the Cortex-M SysTick
 * peripheral and are reused by the driver in `drivers/systick/systick.c`.
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#define TIMER_CTRL_ENABLE           0x00000001u
#define TIMER_CTRL_INTEN            0x00000002u
#define TIMER_CTRL_CLKSRC           0x00000004u
#define TIMER_CTRL_COUNT            0x00010000u

#define TIMER_RELOAD_MASK           0x00FFFFFFu
#define TIMER_CURRENT_MASK          0x00FFFFFFu

#define SYSTICK_2_IRQS_PER_SEC  2u
#define SYSTICK_4_IRQS_PER_SEC  4u
#define SYSTICK_8_IRQS_PER_SEC  8u
#define SYSTICK_16_IRQS_PER_SEC 16u

void timer_enable(void);
void timer_disable(void);
void timer_irq_enable(void);
void timer_irq_disable(void);
void timer_set_period_ms(uint32_t millisec);

#endif /* __TIMER_H__ */

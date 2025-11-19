/* systick.h
 *
 * SysTick helper definitions and API. These constants (control bits,
 * reload masks) match the Cortex-M SysTick peripheral and are reused by
 * the driver in `drivers/systick/systick.c`.
 */

#ifndef __SYSTICK_H__
#define __SYSTICK_H__

/* Compatibility wrapper: forward to `timer.h`. New code should include
 * `timer.h` directly and prefer the `TIMER_*` names. These aliases are
 * kept to avoid breaking code that still uses `systick.h` or `ST*` names.
 */

#include "timer.h"

/* Legacy aliases */
#define STCTRL_ENABLE      TIMER_CTRL_ENABLE
#define STCTRL_INTEN       TIMER_CTRL_INTEN
#define STCTRL_CLKSRC      TIMER_CTRL_CLKSRC
#define STCTRL_COUNT       TIMER_CTRL_COUNT

#define STRELOAD_MASK      TIMER_RELOAD_MASK
#define STCURRENT_MASK     TIMER_CURRENT_MASK

#define SYSTICK_2_IRQS_PER_SEC  2u
#define SYSTICK_4_IRQS_PER_SEC  4u
#define SYSTICK_8_IRQS_PER_SEC  8u
#define SYSTICK_16_IRQS_PER_SEC 16u

#endif /* __SYSTICK_H__ */

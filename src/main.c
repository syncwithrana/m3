#include <stdint.h>

#define SYST_CSR   (*(volatile uint32_t*)0xE000E010)
#define SYST_RVR   (*(volatile uint32_t*)0xE000E014)
#define SYST_CVR   (*(volatile uint32_t*)0xE000E018)

extern volatile uint32_t systick_count;

void safe_delay(void)
{
    for (volatile int i = 0; i < 500000; i++);
}

int main(void)
{
    /* Let CPU stabilize in QEMU for a bit */
    safe_delay();

    /* SAFE SysTick reload (small value QEMU likes) */
    SYST_RVR = 1000;   /* DO NOT use 16000 yet */

    /* Reset current value */
    SYST_CVR = 0;

    /* Enable SysTick: but DO NOT set CLKSOURCE */
    SYST_CSR = (1<<0) | (1<<1);  /* ENABLE | TICKINT */

    /* CLKSOURCE = CPU also causes lockups on some QEMU builds */

    volatile uint32_t last = 0;

    while (1) {
        if (systick_count != last) {
            last = systick_count;
        }
    }
}

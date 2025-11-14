#include <stdint.h>

/* This C function is called from the assembly SysTick_Handler */
volatile uint32_t systick_count = 0;

/* The symbol name must match the one referenced in startup.s */
void SysTick_Handler_C(void) {
    systick_count++;
    /* Keep handler short — no IO here. */
}

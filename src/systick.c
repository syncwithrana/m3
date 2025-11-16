#include <stdint.h>

volatile uint32_t systick_count = 0;

void SysTick_Handler_C(void)
{
    systick_count++;
}

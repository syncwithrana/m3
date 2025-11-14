#include <stdint.h>

volatile uint32_t counter = 0;

void delay(void) {
    for (volatile uint32_t i = 0; i < 100000; i++);
}

int main(void)
{
    while (1) {
        counter++;
        delay();
    }
}

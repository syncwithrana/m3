#include <stdint.h>

volatile uint32_t counter = 0;

void delay() {
    for (volatile int i = 0; i < 500000; i++);
}

int main(void) {
    while (1) {
        counter++;
        delay();
    }
}

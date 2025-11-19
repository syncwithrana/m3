#include <stdint.h>
#include "lm3s6965_memmap.h"
#include "nvic.h"


/* NVIC (Nested Vectored Interrupt Controller) base address
 * The NVIC registers are located at an offset from the peripheral base
 * for the LM3S6965. This is machine-dependent: M3_PERIPHERAL_BASE is
 * defined in `platform/lm3s6965_memmap.h` and must match the target
 * memory map. If porting to a different MCU, update that definition.
 */
#define NVIC_BASE       ((M3_PERIPHERAL_BASE) + 0x00000100u)

/* Minimal NVIC register block used by this driver.
 *
 * Notes:
 * - The arrays named `reserved*` are placeholders for unused register
 *   offsets so the struct fields align to the real hardware addresses.
 * - Priority registers (PRI0..PRI10) pack multiple 8-bit priority
 *   fields per 32-bit word. The current driver does not manipulate
 *   individual priorities, but the layout is provided for clarity.
 * - Keep this struct `packed` to avoid any compiler-inserted padding
 *   that could offset register addresses.
 */
typedef struct __attribute__ ((packed)){
    uint32_t EN0;                   // 0x00 Interrupt 0-31 Set Enable
    uint32_t EN1;                   // 0x04 Interrupt 32-43 Set Enable
    uint32_t reserved[30];          // 0x08-0x7C reserved
    uint32_t DIS0;                  // 0x80 Interrupt 0-31 Clear Enable (Disable)
    uint32_t DIS1;                  // 0x84 Interrupt 32-43 Clear Enable (Disable)
    uint32_t reserved1[30];         // 0x88-0xFC reserved
    uint32_t PEND0;                 // 0x100 Interrupt 0-31 Set Pending
    uint32_t PEND1;                 // 0x104 Interrupt 32-43 Set Pending
    uint32_t reserved2[30];         // 0x108-0x17C reserved
    uint32_t UNPEND0;               // 0x180 Interrupt 0-31 Clear Pending
    uint32_t UNPEND1;               // 0x184 Interrupt 32-43 Clear Pending
    uint32_t reserved3[30];         // 0x188-0x1FC reserved
    uint32_t ACTIVE0;               // 0x200 Interrupt 0-31 Active Bit
    uint32_t ACTIVE1;               // 0x204 Interrupt 32-43 Active Bit
    uint32_t reserved4[62];         // 0x208-0x2FC reserved
    uint32_t PRI0;                  // 0x300 Interrupt 0-3 Priority (packed)
    uint32_t PRI1;                  // 0x304 Interrupt 4-7 Priority
    uint32_t PRI2;                  // 0x308 Interrupt 8-11 Priority
    uint32_t PRI3;                  // 0x30C Interrupt 12-15 Priority
    uint32_t PRI4;                  // 0x310 Interrupt 16-19 Priority
    uint32_t PRI5;                  // 0x314 Interrupt 20-23 Priority
    uint32_t PRI6;                  // 0x318 Interrupt 24-27 Priority
    uint32_t PRI7;                  // 0x31C Interrupt 28-31 Priority
    uint32_t PRI8;                  // 0x320 Interrupt 32-35 Priority
    uint32_t PRI9;                  // 0x324 Interrupt 36-39 Priority
    uint32_t PRI10;                 // 0x328 Interrupt 40-43 Priority
} nvic_regs;
typedef nvic_regs irqc_regs;

/* Pointer to the NVIC register block. Declared `volatile` because
 * the hardware can change these values asynchronously (e.g., pending
 * bits, active bits). Casting from the base constant ties the C
 * representation to the correct memory-mapped address.
 */
static volatile irqc_regs *irq_controller = (irqc_regs *)NVIC_BASE;

void irq_enable(uint32_t vector_num)
{
    /*
     * Enable NVIC interrupt for the given vector number.
     *
     * Parameters:
     *  - vector_num: platform IRQ vector identifier (see IRQ_* in headers).
     *
     * Behavior / notes:
     *  - The LM3S NVIC groups interrupts into banks (0-31 in EN0, 32+ in EN1).
     *  - This function computes the bit within the correct bank and sets it.
     *  - No range error is reported here; callers should pass a valid IRQ number.
     *  - This operation is a non-atomic read-modify-write to the register.
     *    On this MCU the register is 32-bit and bitwise OR is acceptable. If
     *    multiple cores or concurrent writers existed, this would require
     *    additional synchronization (not applicable for single-core Cortex-M3).
     */
    /* Interrupts 0-31 enabled via EN0 */
    if (vector_num >= IRQ_GPIOA && vector_num <= IRQ_ADC0_SEQ1) {
        irq_controller->EN0 |= (1u << (vector_num - IRQ_GPIOA));
    }
    /* Interrupts 32-43 enabled via EN1 */
    else if (vector_num >= IRQ_ADC0_SEQ2 && vector_num <= IRQ_HIBERNATE) {
        irq_controller->EN1 |= (1u << (vector_num - IRQ_ADC0_SEQ2));
    }

}

void nvic_irq_disable(uint32_t vector_num)
{
    /*
     * Disable NVIC interrupt for the given vector number.
     *
     * This writes to the DISx register which clears the corresponding
     * enable bit. The semantics mirror `irq_enable`.
     */
    /* Interrupts 0-31 disabled via DIS0 */
    if (vector_num >= IRQ_GPIOA && vector_num <= IRQ_ADC0_SEQ1) {
        irq_controller->DIS0 |= (1u << (vector_num - IRQ_GPIOA));
    }
    /* Interrupts 32-43 disabled via DIS1 */
    else if (vector_num >= IRQ_ADC0_SEQ2 && vector_num <= IRQ_HIBERNATE) {
        irq_controller->DIS1 |= (1u << (vector_num - IRQ_ADC0_SEQ2));
    }
}

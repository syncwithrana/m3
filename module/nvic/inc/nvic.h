#ifndef __NVIC_H__
#define __NVIC_H__

#define NUM_EXCEPTIONS      60u
#define NUM_IRQS            44u

#define FAULT_NMI           2u
#define FAULT_HARD          3u
#define FAULT_MEM_MGMT      4u
#define FAULT_BUS           5u
#define FAULT_USAGE         6u
#define FAULT_SVCALL        11u
#define FAULT_DEBUG         12u
#define FAULT_PENDSV        14u
#define FAULT_SYS_TICK      15u
#define IRQ_GPIOA           16u
#define IRQ_GPIOB           17u
#define IRQ_GPIOC           18u
#define IRQ_GPIOD           19u
#define IRQ_GPIOE           20u
#define IRQ_UART0           21u
#define IRQ_UART1           22u
#define IRQ_SSI0            23u
#define IRQ_I2C0            24u
#define IRQ_PWM_FAULT       25u
#define IRQ_PWM0            26u
#define IRQ_PWM1            27u
#define IRQ_PWM2            28u
#define IRQ_QEI0            29u
#define IRQ_ADC0_SEQ0       30u
#define IRQ_ADC0_SEQ1       31u
#define IRQ_ADC0_SEQ2       32u
#define IRQ_ADC0_SEQ3       33u
#define IRQ_WATCHDOG0       34u
#define IRQ_TIMER0A         35u
#define IRQ_TIMER0B         36u
#define IRQ_TIMER1A         37u
#define IRQ_TIMER1B         38u
#define IRQ_TIMER2A         39u
#define IRQ_TIMER2B         40u
#define IRQ_ACOMP0          41u
#define IRQ_ACOMP1          42u
#define IRQ_SYS_CTL         44u
#define IRQ_FLASH_CTL       45u
#define IRQ_GPIOF           46u
#define IRQ_GPIOG           47u
#define IRQ_UART2           49u
#define IRQ_TIMER3A         51u
#define IRQ_TIMER3B         52u
#define IRQ_I2C1            53u
#define IRQ_QEI1            54u
#define IRQ_ETH             58u
#define IRQ_HIBERNATE       59u
void nvic_irq_enable(uint32_t vector_num);
void nvic_irq_disable(uint32_t vector_num);
#endif /* __NVIC_H__ */

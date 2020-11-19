#ifndef DELAY_H
#define DELAY_H

void _delay_ms(uint16_t);
void SysTick_init_ms(int);
void SysTick_Handler(void);

#endif
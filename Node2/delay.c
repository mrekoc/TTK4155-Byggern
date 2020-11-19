/*
Utdelt fra assistent
*/


#include "sam.h"
#include "delay.h"

volatile uint32_t wait_ticks = 0;
#define  F_CPU 84000000

void _delay_ms(uint16_t ms){
	wait_ticks = ms;
	SysTick_init_ms(1);
	while (wait_ticks != 0);	
}

void SysTick_init_ms(int period){
	
	SysTick->LOAD = (period * 10500 &SysTick_LOAD_RELOAD_Msk) - 1;
	
	SysTick->VAL = 0;
	
	NVIC_SetPriority(SysTick_IRQn, 1);
	
	SysTick->CTRL = (0b0 << SysTick_CTRL_CLKSOURCE_Pos) &
		SysTick_CTRL_COUNTFLAG_Msk |
		(0b1 << SysTick_CTRL_TICKINT_Pos & SysTick_CTRL_TICKINT_Msk) |
		(0b1 << SysTick_CTRL_ENABLE_Pos & SysTick_CTRL_ENABLE_Msk);
}

void SysTick_Handler(void) {
	if (wait_ticks != 0) {
		wait_ticks--;
	} else SysTick->CTRL = 0;
}
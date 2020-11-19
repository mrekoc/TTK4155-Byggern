#include "ADC_driver.h"
#include "sam.h"
#include "node2_bit_macros.h"
#include "global.h"

goalFlag=0;

void ADC_init(void){
	
	set_bit(ADC, ADC_CR, 0);  // Hardware like reset on ADC
	set_bit(PMC, PMC_PCER1, 5);  // PIOD PID37 = ADC enable peripheral clk
	set_bit(PIOA, PIO_PDR, 2);	// Disable io, enable peripheral = ADC

	ADC->ADC_MR=ADC_MR_TRGEN_EN //enable trigger
	|ADC_MR_TRGSEL_ADC_TRIG0 //Enable external trigger
	|ADC_MR_FREERUN_ON; //Activate freerun mode
	
	ADC-> ADC_EMR |= ADC_EMR_CMPMODE_LOW;  // Generate interrupt if value is lower than low threshold
	ADC-> ADC_EMR &=  ~(ADC_EMR_CMPALL);	// Do not generate interrupt for all channels 
 	ADC->ADC_EMR |= ADC_EMR_CMPSEL(0) ;	// Channel 0 is following for an interrupt
    
	__disable_irq(); //No interrupts when writing to ISER register
 	NVIC->ISER[(((uint32_t)(int32_t)ID_ADC) >> 5UL)] |= (uint32_t)(1UL << (((uint32_t)(int32_t)ID_ADC) & 0x1FUL)); //enable ADC interrupt in NVIC
	NVIC_SetPriority((IRQn_Type)ID_ADC,4); //Set priority of ADC interrupt
	__enable_irq();

	ADC->ADC_IDR = 0xFFFFFFFF;  // Disable all of the interrupts on ADC
 	ADC->ADC_IER |= ADC_IER_COMPE; //Enable compare interrupt

 	ADC->ADC_CWR |= 0x100;  // low Threshold
 	ADC->ADC_CWR |= (0x101 << 16); // high Threshold (not used)
	
	set_bit(ADC, ADC_CHER, 0);   // Channel enable 0
	set_bit(ADC, ADC_CR, 1);     // ADC control register start analog conversion
	
	ADC->ADC_WPMR = 0x414443 << 8 | ADC_WPMR_WPEN;
}


void ADC_Handler(void){
		(ADC->ADC_ISR & 0x04000000); //Read ISR, clear the interrupt bit
		goalFlag=1; 
}
	
void ADC_update_goal(int *goal){
	if(goalFlag==1 && ADC->ADC_CDR[0] > 0x400){ //Count goal when ball no longer blocks IR beam
		(*goal)+=1;
		goalFlag=0;
		_delay_ms(1);
		}
	}
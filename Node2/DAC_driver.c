
#include "sam.h"
#include "DAC_driver.h"
#include "node2_bit_macros.h"

void DAC_init(void){
	
	DACC->DACC_CDR = 0; //Starter uten å kjøre
	DACC->DACC_CR = DACC_CR_SWRST;  // Software resett
	set_bit(PMC, PMC_PCER1, 6);  // PIOD PID38 = DACC enable peripheral clk
	set_bit(PIOB, PIO_PDR, 15);		// Enable peripheral control on DAC 0
	
	DACC->DACC_MR = DACC_MR_TRGEN_DIS 
	| DACC_MR_WORD_HALF
	| DACC_MR_USER_SEL_CHANNEL0;
	DACC->DACC_CHER = DACC_CHER_CH0;
	
	DACC->DACC_CDR = 0; //Starter uten å kjøre
}


void DAC_control_speed(uint32_t velo){
	DACC->DACC_CDR = velo;
}
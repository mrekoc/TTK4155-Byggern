#include "solenoid_driver.h"
#include "sam.h"
#include "node2_bit_macros.h"

void solenoid_init(){
	//-----solenoid inits-------
	set_bit(PMC, PMC_PCER0, 12);
	set_bit(PIOB, PIO_PER, 26);
	set_bit(PIOB, PIO_OER, 26);
}

void solenoid_drive(uint8_t button){
	
 	if(button != 1){
 		set_bit(PIOB, PIO_SODR, 26);
 	} else {
 		set_bit(PIOB, PIO_CODR, 26);
 	}
			
}
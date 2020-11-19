#include "PWM_driver.h"
#include "node2_bit_macros.h"
#include "sam.h"

#define LOW_LIMIT 900
#define HIGH_LIMIT 2100


void PWM_init() {
	PIOC->PIO_PDR |= PIO_PDR_P19;   //deactivate I/O function on PWM pin (enable peripheral function)
	PIOC->PIO_ABSR |= PIO_ABSR_P19; //PIOC19 = pin 44 on shield, Choose the deactivated PIOB

	PWM->PWM_CLK = PWM_CLK_DIVA(28) | PWM_CLK_PREA(0); //set clok frequency CLCKA = 84/28 = 3
	PMC->PMC_PCER1 |= PMC_PCER1_PID36; //enable timer counter in power management controller
	REG_PWM_CMR5 = 0xB | 1 << 9; //CLKA=0b1011, Channel polarity
	REG_PWM_CPRD5 = 60000; //Set periode to 20 ms(1/3Mhz*60000)
	REG_PWM_CDTY5 = 2700; //Set duty cycle to 60000*0.045=2700
	PWM->PWM_ENA |= PWM_ENA_CHID5;

}

void PWM_set_DC(float DC) {
	if(DC<LOW_LIMIT || DC>HIGH_LIMIT){
		return;
	} //Exits the function if out of bounds
	REG_PWM_CDTY5 = 3*DC; //Set pulse width based on DC
}

float PWM_convert_from_can(float pos){
	float pwm_dc;
	pwm_dc = ((float)pos/255)*(2100 - 900) + 900;
	return pwm_dc;
}
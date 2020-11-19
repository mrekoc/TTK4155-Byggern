#include "sam.h"
#include "global.h"
#include "motor_driver.h"
#include "PI_controller_driver.h"
#include "node2_bit_macros.h"
#include "can_controller.h"
#include "can_interrupt.h"

int32_t sum_prev_e = 0;
int16_t integral_gain=1;

void PI_feedforward(int16_t slider){
	if(slider>0){
		motor_control(3, 10000);
	}
	else {motor_control(0, 10000);}
}


void PI_timer_counter_init() {
	PMC->PMC_PCER0 |= 0x1u << ID_TC0; //enable timer counter in power management controller

	set_bit(PIOB, PIO_PDR, 25); //deactivate I/O function on PWM pin (enable peripheral function)
	set_bit(PIOB, PIO_ABSR, 25); //Choose the deactivated PIOB

	TC0->TC_CHANNEL[0].TC_CMR|=TC_CMR_TCCLKS_TIMER_CLOCK1; //TCCLKS: internal MCK/2 clock signal
	TC0->TC_CHANNEL[0].TC_CMR|=TC_CMR_WAVE; //WAVE: enable waveform mode
	TC0->TC_CHANNEL[0].TC_CMR|=TC_CMR_WAVSEL_UP_RC; //WAVSEL: enable mode 10 wave type,
	TC0->TC_CHANNEL[0].TC_CMR|=TC_CMR_ACPA_CLEAR; //ACPA: clear RA compare effect on TIOA (I/O A)
	TC0->TC_CHANNEL[0].TC_CMR|=TC_CMR_ACPC_SET; //ACPA: set RA compare effect on TIOA (I/O A)

	TC0->TC_CHANNEL[0].TC_RC=8400000; //Sets master clock frequency, the end of CV
	TC0->TC_CHANNEL[0].TC_RA=100000; //Sets register A value. Initializing on lowest DC.
	//Out=0 for CV: 0-37800, and one for CV: 37800-840000. DC på 4,5%, this means PW at 0.045*20ms=0.9.

	TC0->TC_CHANNEL[0].TC_IER=TC_IER_CPCS | TC_IER_CPAS; //CPCS: enable RC compare, CPAS: enable RA compare interrupt

	__disable_irq(); //Disable interrupts when writing to NVIC
	NVIC->ISER[(((uint32_t)(int32_t)ID_TC0) >> 5UL)] |= (uint32_t)(1UL << (((uint32_t)(int32_t)ID_TC0) & 0x1FUL)); //Enable NVIC interrupts on TC0
	NVIC_SetPriority((IRQn_Type)ID_TC0,3); //Set priority of TC0 interrupts
	__enable_irq(); //Enable NVIC interrupts

	TC0->TC_CHANNEL[0].TC_CCR=TC_CCR_SWTRG|TC_CCR_CLKEN;//Start the clock from software, and enable the clock.
}

void TC0_Handler(void){
	uint32_t overflow_check;
	overflow_check=TC0->TC_CHANNEL[0].TC_SR;
	pi_regulator();
}


void p_regulator(int16_t ref, int16_t measurement, int16_t k_p){
	int16_t error = 0;
	int dir;
	error = ref - measurement;
	if (error < 0){
		motor_control(0, (-(error)*k_p));
	} else 
	{ 
		motor_control(3,((error)*k_p));
	}
}

void pi_regulator(){
	int16_t ref, measure; //initializing variables
	int16_t error = 0;
	int16_t input_vector = 0;
	uint8_t dir;
	float time_step = 0.2;
	float time_step_corr=0.1;
	int16_t k_p; 
	uint32_t speed;
	
	integral_gain = 0; //initializing gain factors
	k_p = 40;
	
	
	ref = pi_get_ref(); //get reference
	measure = pi_get_measure(); //get measurement
	error = ref - measure; //calculate error
	
	sum_prev_e=sum_prev_e+error; //integrate (sum) the deviation
	
// 	if(sum_prev_e>500){sum_prev_e=0;} //stabilizing if-functions
// 	if(abs(error)<5){error=0;}
	
	input_vector = k_p * error + time_step_corr*time_step*integral_gain*(sum_prev_e); //Calculating u
	
		
	if (error < 0){ //use u to drive the motor
		speed=-input_vector;
		motor_control(0, speed);
	} else
	{
		speed=input_vector;
		motor_control(3, speed);
	}
	
}


int16_t pi_get_measure(){
	return encoder_to_pos(motor_read_encoder());
}

int16_t pi_get_ref(){
	CAN_MESSAGE PCB_information;
	get_can_message(&PCB_information);
	int16_t ref = motor_CAN_to_pos(PCB_information.data[0]);
	return ref;
}
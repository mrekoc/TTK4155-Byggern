
#include "sam.h"
#include "motor_driver.h"
#include "DAC_driver.h"
#include "node2_bit_macros.h"
#include "delay.h"


uint8_t encoderdataLSB;
uint8_t encoderdataMSB;
uint16_t encoderdata;


void motor_init(void){
	DAC_init();
	
	set_bit(PMC, PMC_PCER0, 14);  // PIOD PID14 = PIOD enable peripheral clk
	
	set_bit(PIOD, PIO_PER, 0);
	set_bit(PIOD, PIO_PER, 1);
	set_bit(PIOD, PIO_PER, 2);
	set_bit(PIOD, PIO_PER, 9);	// Enable PIO control of motor control pins
	set_bit(PIOD, PIO_PER, 10);
	
	set_bit(PIOD, PIO_PUDR, 0);
	set_bit(PIOD, PIO_PUDR, 1);
	set_bit(PIOD, PIO_PUDR, 2);
	set_bit(PIOD, PIO_PUDR, 9);	// Enable PIO control of motor control pins
	set_bit(PIOD, PIO_PUDR, 10);

	set_bit(PIOD, PIO_SODR, 0); //Not output enable

	set_bit(PIOD, PIO_OER, 0);
	set_bit(PIOD, PIO_OER, 1);
	set_bit(PIOD, PIO_OER, 2);
	set_bit(PIOD, PIO_OER, 9);	// Enable motor control pins as output
	set_bit(PIOD, PIO_OER, 10);
	
	//Set motor control pins
	set_bit(PIOD, PIO_CODR, 0); //Not output enable
	set_bit(PIOD, PIO_SODR, 1); //Not reset
	set_bit(PIOD, PIO_CODR, 2); //Select HI/LOW byte of encoder
	set_bit(PIOD, PIO_SODR, 9); //Enable
	set_bit(PIOD, PIO_CODR, 10);  //DIR
	
	
	set_bit(PMC, PMC_PCER0, 13);// PIOD PID13 = PIOC enable peripheral clk	
	set_multiple_bits( PIOC, PIO_PER, 1, 0xff);  // Disable io, enable peripheral = ADC
	set_multiple_bits( PIOC, PIO_ODR, 1, 0xff);	// Disable output function	
	set_multiple_bits( PIOC, PIO_IFER, 1, 0xff);
	set_multiple_bits( PIOC, PIO_PUDR, 1, 0xff);
	
	set_bit(PIOD, PIO_CODR, 1); //Toggle reset
	_delay_ms(20); //wait > 20 ms
	set_bit(PIOD, PIO_SODR, 1);
		
}

uint8_t motor_set_direction(uint8_t dir){
	switch(dir) {
		case 3: set_bit(PIOD, PIO_SODR, 10); return 0; //RIGHT
		case 0: set_bit(PIOD, PIO_CODR, 10); return 0; //LEFT 
		default: return 1;
	}
}


void motor_control(uint8_t dir, uint32_t speed){
	if(!motor_set_direction(dir)) {
		DAC_control_speed(speed);
	}
	else{DAC_control_speed(0);}
}

uint16_t motor_joy_to_speed(uint8_t x_pos){
	uint16_t speed=(x_pos/255)*10000;
	return speed;
}


int16_t motor_CAN_to_pos(uint8_t temp){
	return (int16_t)((temp - 126) * 100 / 126);
}

uint16_t motor_read_encoder(void){
	/*motor encoder, it follws the motorbox user guides walk through*/
	uint16_t data_exp;
	uint8_t low_byte;
	uint8_t high_byte;
	
	set_bit_hard(PIOD, PIO_CODR, PIO_PD0);
	set_bit_hard(PIOD, PIO_CODR, PIO_PD2);
	
	_delay_ms(1);
	
	high_byte = PIOC->PIO_PDSR;
	//printf("HIGH BYTE: %d\n\r", high_byte);
	
	
	set_bit_hard(PIOD, PIO_SODR, PIO_PD2);
	_delay_ms(1);
	
	low_byte = PIOC->PIO_PDSR;
	//printf("LOW BYTE: %d\n\r", low_byte);
	
	set_bit_hard(PIOD, PIO_CODR, PIO_PD1);
	set_bit_hard(PIOD, PIO_SODR, PIO_PD1);
	
	data_exp = (high_byte<<8) | low_byte;
	
	set_bit_hard(PIOD, PIO_SODR, PIO_PD0);
	
	return data_exp;
}


int16_t encoder_to_pos(uint16_t val){
	if(val < 0){
		return (-(int16_t) val)*255/20000;
	} else {
		return -((int16_t) val)*255/20000;
	}
}


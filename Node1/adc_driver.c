#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "adc_driver.h"

#ifndef ADC_ADDR
#define ADC_ADDR 0x1400
#endif 

volatile char* ext_adc = ADC_ADDR;	//pointer to adc address
volatile char ADC_data;


void adc_init(void){
	
	DDRB |= (1 << 0);
	
	TCCR0 |= (1 << WGM01); // mode CTC
	
	TCCR0 |= (1 << COM00); // compare mode toggle on match
	
	TCCR0 |=  (1 << CS00); // clock select no pre-scaling
	
	OCR0 = 3; // Output compare to toggle.
	
}

char ADC_read(channel_t channel) {
		char data;
		switch (channel){
			case CHANNEL0:
			data = 0b10000000;
			break;
			case CHANNEL1:
			data = 0b10000001;
			break;
			case CHANNEL2:
			data = 0b10000010;
			break;
			case CHANNEL3:
			data = 0b10000011;
			break;
			default: 
			data=0x00;
			break;
		}
		
		ext_adc[0x00] = data;
		_delay_ms(1);
		ADC_data = ext_adc[0x00];
		
		return ADC_data;

}
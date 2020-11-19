#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "adc_driver.h"
#include "joystick_driver.h"
#include "usart.h"
#include "slider_driver.h"


uint8_t slider_get_left_slider(){
	return ADC_read(CHANNEL2);
}

uint8_t slider_get_right_slider(){
	return ADC_read(CHANNEL3);
}

uint8_t slider_get_right_button(){
	DDRE &= ~(1<< DDE2);
	return PINE >> 2;
}






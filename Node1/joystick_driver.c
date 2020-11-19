#include <avr/io.h>
#include <util/delay.h>
#include "adc_driver.h"
#include "joystick_driver.h"
#include "usart.h"


uint8_t get_x_pos_joy(){
	uint8_t xpos=(uint8_t) ADC_read(CHANNEL0);
	return xpos;
	}
	
uint8_t get_y_pos_joy(){
	uint8_t ypos=(uint8_t) ADC_read(CHANNEL1);
	return ypos;
	}


direction get_direction (){
	direction dir = ERROR;  // joystick direction
	uint8_t temp_x = get_x_pos_joy();
	uint8_t temp_y = get_y_pos_joy();
	uint8_t low_cutoff = 150;
	uint8_t high_cutoff = 230;
	if ((temp_x < 210 && temp_y < high_cutoff) && (temp_x > low_cutoff && temp_y > low_cutoff)){ 
		dir = NEUTRAL;
	} else if ((temp_x < low_cutoff && temp_y < high_cutoff) && (temp_x < low_cutoff && temp_y > low_cutoff)){
		dir = LEFT;
	} else if ((temp_x > low_cutoff && temp_y > high_cutoff) && (temp_x < 210 && temp_y > 210)){
		dir = UP;
	} else if ((temp_x > high_cutoff && temp_y < high_cutoff) && (temp_x > high_cutoff && temp_y > low_cutoff)){
		dir = RIGHT;
	} else if ((temp_x > low_cutoff && temp_y < low_cutoff) && (temp_x < high_cutoff && temp_y < low_cutoff)){
		dir = DOWN;
	}
	return dir;
}

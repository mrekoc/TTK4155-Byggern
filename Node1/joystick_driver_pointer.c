#include <avr/io.h>
#include <util/delay.h>
#include "adc_driver.h"
#include "joystick_driver.h"
#include "joystick_driver_pointer.h"
#include "usart.h"


void update_joystick_state_pointer(joystick* my_joystick, channel_t channel0, channel_t channel1){
	ADC_read(channel0);  // read data from channel 0 and 1
	_delay_ms(1);
	uint8_t temp_xpos= ADC_get_data();
	
	ADC_read(channel1);
	_delay_ms(1);
	uint8_t temp_ypos= ADC_get_data();
	
	my_joystick->X_POS = (temp_xpos - 126) * 100 / 126 - my_joystick->X_OFFSET;
	my_joystick->Y_POS = (temp_ypos - 126) * 100 / 126 - my_joystick->Y_OFFSET;
	
	
}

direction get_direction_pointer(joystick myjoystick, channel_t channel0, channel_t channel1){
	update_joystick_state_pointer(&myjoystick, channel0, channel1);
	direction dir = ERROR;  // joystick direction
	if ((myjoystick.Y_POS < 10 && myjoystick.Y_POS > -100) && (myjoystick.X_POS < 10 && myjoystick.X_POS > -100)){
		dir = NEUTRAL;
		} else if ((myjoystick.X_POS < -100) && (myjoystick.Y_POS < 25 && myjoystick.Y_POS > -100)){
		dir = LEFT;
		} else if ((myjoystick.Y_POS > 0) && (myjoystick.X_POS < 25 && myjoystick.X_POS > -100)){
		dir = UP;
		} else if ((myjoystick.X_POS > 0) && (myjoystick.Y_POS < 25 && myjoystick.Y_POS > -100)) {
		dir = RIGHT;
		} else if ((myjoystick.Y_POS < 0) && (myjoystick.X_POS < 25 && myjoystick.X_POS > -100)) {
		dir = DOWN;
	}
	return dir;
}

void joystick_init_pointer(joystick *myjoystick){
	//ADC_read(CHANNEL0);
	//_delay_ms(1);
	//myjoystick->X_OFFSET= (ADC_get_data() - 126)*100/126; // calibrates offset
	
	
	//ADC_read(CHANNEL1);
	//_delay_ms(1);
	//myjoystick->Y_OFFSET= (ADC_get_data() - 126)*100/126;
}

void test(joystick myjoystick){}


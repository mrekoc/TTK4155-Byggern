#include <avr/io.h>
#include "adc_driver.h"

#ifndef JOYSTICK_POINTER_33
#define JOYSTICK_POINTER_33

void update_joystick_state_pointer(joystick*, channel_t, channel_t);
direction get_direction_pointer(joystick, channel_t, channel_t);
void joystick_init_pointer(joystick*);
void test(joystick);

#endif


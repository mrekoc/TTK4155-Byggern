#include <avr/io.h>
#include "adc_driver.h"

#ifndef JOYSTICK_33
#define JOYSTICK_33

typedef enum {LEFT, UP, DOWN, RIGHT, NEUTRAL, ERROR} direction;
direction get_direction ();
uint8_t get_x_pos_joy();
uint8_t get_y_pos_joy();

#endif


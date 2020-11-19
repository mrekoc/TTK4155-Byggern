#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H


void motor_init(void);
uint8_t motor_set_direction(uint8_t);
void motor_control(uint8_t, uint32_t);
uint16_t motor_joy_to_speed(uint8_t);
int16_t motor_CAN_to_pos(uint8_t);
uint16_t motor_read_encoder();
int16_t encoder_to_pos(uint16_t);

#endif

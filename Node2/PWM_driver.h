#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H


void PWM_init(void);
void PWM_set_DC(float);
void PWM_interrupt_handler(void);
float PWM_convert_from_can(float);

#endif
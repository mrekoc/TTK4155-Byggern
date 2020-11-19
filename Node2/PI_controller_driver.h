#ifndef PI_CONTROLLER_DRIVER_H
#define PI_CONTROLLER_DRIVER_H

void pi_feedforward(int16_t);
void PI_timer_counter_init(void);
void TC2_Handler(void);
void p_regulator(int16_t , int16_t, int16_t);
void pi_regulator();
int16_t pi_get_measure();
int16_t pi_get_ref();

#endif
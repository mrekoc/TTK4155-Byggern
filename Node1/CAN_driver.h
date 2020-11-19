#include <stdio.h>
#ifndef CAN_DRIVER_GUARD
#define CAN_DRIVER_GUARD

typedef struct can_message{
	uint16_t ID;
	uint8_t length;
	uint8_t data[8];
}can_message;

void CAN_init();
void CAN_send(can_message*);
void CAN_interrupt_init();
int CAN_interrupt_handler(can_message*);
void CAN_receive(can_message*);


#endif

